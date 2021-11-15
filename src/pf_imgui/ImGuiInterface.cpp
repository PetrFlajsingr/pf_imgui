//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "serialization.h"
#include <implot.h>
#include <utility>

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig, bool enableMultiViewport,
                               const std::filesystem::path &iconFontDirectory, const Flags<IconPack> &enabledIconPacks,
                               float iconSize)
    : Renderable("imgui_interface"), io(baseInit(flags | ImGuiConfigFlags_DockingEnable
                                                 | (enableMultiViewport ? ImGuiConfigFlags_ViewportsEnable : 0))),
      fontManager(*this, iconFontDirectory, enabledIconPacks, iconSize), config(std::move(tomlConfig)) {}

ImGuiIO &ImGuiInterface::baseInit(ImGuiConfigFlags flags) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  auto &imguiIo = ImGui::GetIO();
  imguiIo.ConfigFlags |= flags;
  ImGui::StyleColorsDark();
  return imguiIo;
}

ImGuiIO &ImGuiInterface::getIo() const { return io; }

ModalDialog &ImGuiInterface::createDialog(const std::string &elementName, const std::string &caption) {
  auto dialog = std::make_unique<ModalDialog>(*this, elementName, caption);
  const auto ptr = dialog.get();
  dialogs.emplace_back(std::move(dialog));
  return *ptr;
}

AppMenuBar &ImGuiInterface::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<AppMenuBar>("app_menu_bar"); }
  return *menuBar;
}

bool ImGuiInterface::hasMenuBar() const { return menuBar != nullptr; }

AppStatusBar &ImGuiInterface::createStatusBar(const std::string &barName) {
  statusBar = std::make_unique<AppStatusBar>(barName);
  return *statusBar;
}

void ImGuiInterface::removeStatusBar() { statusBar = nullptr; }

const toml::table &ImGuiInterface::getConfig() const { return config; }

void ImGuiInterface::updateConfig() {
  //config.clear();
  std::ranges::for_each(windows, [this](auto &window) {
    auto serialisedTree = serializeImGuiTree(*window);
    for (const auto &item : serialisedTree) { config.insert_or_assign(item.first, item.second); }
    if (menuBar != nullptr) {
      auto serialisedAppBar = serializeImGuiTree(*menuBar);
      for (const auto &item : serialisedAppBar) { config.insert_or_assign(item.first, item.second); }
    }
    //config.insert(serialised.begin(), serialised.end());
  });
}

void ImGuiInterface::setStateFromConfig() {
  const auto serialiseSubtree = [this](Renderable &root) {
    traverseImGuiTree(root, [this](Renderable &renderable) {
      if (auto ptrSavable = dynamic_cast<Savable *>(&renderable); ptrSavable != nullptr) {
        if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr) {
          if (config.contains(ptrElement->getName())) {
            ptrSavable->unserialize(*config[ptrElement->getName()].as_table());
          }
        }
      }
    });
  };
  if (menuBar != nullptr) { serialiseSubtree(*menuBar); }
  std::ranges::for_each(windows, [this, &serialiseSubtree](auto &window) { serialiseSubtree(*window); });
}
void ImGuiInterface::renderDialogs() {
  std::ranges::for_each(fileDialogs, [](auto &dialog) { dialog.render(); });
  if (const auto iter = std::ranges::find_if(fileDialogs, [](auto &dialog) { return dialog.isDone(); });
      iter != fileDialogs.end()) {
    fileDialogs.erase(iter);
  }
  std::ranges::for_each(dialogs, [](auto &dialog) { dialog->render(); });
  if (const auto iter = std::ranges::find_if(dialogs, [](auto &dialog) { return dialog->isClosed(); });
      iter != dialogs.end()) {
    dialogs.erase(iter);
  }
}
bool ImGuiInterface::isWindowHovered() const { return io.WantCaptureMouse; }

bool ImGuiInterface::isKeyboardCaptured() const { return io.WantCaptureKeyboard; }

Window &ImGuiInterface::createWindow(const std::string &windowName, std::string title) {
  windows.emplace_back(std::make_unique<Window>(windowName, std::move(title)));
  return *windows.back();
}

void ImGuiInterface::removeWindow(const std::string &windowName) {
  windows.erase(std::ranges::find(windows, windowName, [](const auto &window) { return window->getName(); }));
}

void ImGuiInterface::removeWindow(const Window &window) {
  windows.erase(std::ranges::find(windows, &window, [](const auto &window) { return window.get(); }));
}

std::optional<std::reference_wrapper<Window>> ImGuiInterface::windowByName(const std::string &windowName) {
  if (auto window = findIf(getWindows() | ranges::views::addressof,
                           [windowName](const auto &window) { return window->getName() == windowName; });
      window.has_value()) {
    return **window;
  } else {
    return std::nullopt;
  }
}

void ImGuiInterface::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (hasMenuBar()) { menuBar->render(); }
  std::ranges::for_each(windows, [](auto &window) { window->render(); });
  std::ranges::for_each(dragNDropGroups, [](auto &group) { group.frame(); });
  if (statusBar != nullptr) { statusBar->render(); }
  renderDialogs();
  ImGui::RenderNotifications(notifications);
}

void ImGuiInterface::removeDialog(ModalDialog &dialog) {
  if (const auto iter = std::ranges::find_if(dialogs, [&dialog](const auto &ptr) { return ptr.get() == &dialog; });
      iter != dialogs.end()) {
    dialogs.erase(iter);
  }
}
DragNDropGroup &ImGuiInterface::createDragNDropGroup() { return dragNDropGroups.emplace_back(); }

FontManager &ImGuiInterface::getFontManager() { return fontManager; }

void ImGuiInterface::showNotification(NotificationType type, std::string_view message,
                                      std::chrono::milliseconds dismissTime) {
  ImGui::InsertNotification({static_cast<int>(type), std::string(message).c_str(), dismissTime.count()}, notifications);
}

void ImGuiInterface::showNotification(NotificationType type, std::string_view title, std::string_view message,
                                      std::chrono::milliseconds dismissTime) {
  ImGuiToast toast{static_cast<int>(type), std::string(message).c_str(), dismissTime.count()};
  toast.set_title(std::string{title}.c_str());
  ImGui::InsertNotification(toast, notifications);
}

}// namespace pf::ui::ig