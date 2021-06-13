//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "serialization.h"
#include "src/pf_imgui/dialogs/ModalDialog.h"
#include <imgui_internal.h>
#include <implot.h>
#include <pf_common/algorithms.h>
#include <range/v3/view/addressof.hpp>
#include <utility>

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig,
                               const std::filesystem::path &iconFontDirectory, Flags<IconPack> enabledIconPacks,
                               float iconSize)
    : Renderable("imgui_interface"), fontManager(*this, iconFontDirectory, enabledIconPacks, iconSize),
      io(baseInit(flags)), config(std::move(tomlConfig)) {}

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
const toml::table &ImGuiInterface::getConfig() const { return config; }

void ImGuiInterface::updateConfig() {
  //config.clear();
  std::ranges::for_each(windows, [this](auto &window) {
    auto serialised = serializeImGuiTree(*window);
    for (const auto &item : serialised) { config.insert_or_assign(item.first, item.second); }
    //config.insert(serialised.begin(), serialised.end());
  });
}

void ImGuiInterface::setStateFromConfig() {
  std::ranges::for_each(windows, [this](auto &window) {
    traverseImGuiTree(*window, [this](Renderable &renderable) {
      if (auto ptrSavable = dynamic_cast<Savable *>(&renderable); ptrSavable != nullptr) {
        if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr) {
          if (config.contains(ptrElement->getName())) {
            ptrSavable->unserialize(*config[ptrElement->getName()].as_table());
          }
        }
      }
    });
  });
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

void ImGuiInterface::removeWindow(const std::string &name) {
  if (auto iter = std::ranges::find_if(windows, [name](const auto &window) { return window->getName() == name; });
      iter != windows.end()) {
    windows.erase(iter);
  }
}
std::optional<std::reference_wrapper<Window>> ImGuiInterface::windowByName(const std::string &name) {
  if (auto window = findIf(getWindows() | ranges::views::addressof,
                           [name](const auto &window) { return window->getName() == name; });
      window.has_value()) {
    return **window;
  } else {
    return std::nullopt;
  }
}

void ImGuiInterface::renderImpl() {
  std::ranges::for_each(windows, [](auto &window) { window->render(); });
  std::ranges::for_each(dragNDropGroups, [](auto &group) { group.frame(); });
}

void ImGuiInterface::removeDialog(ModalDialog &dialog) {
  if (const auto iter = std::ranges::find_if(dialogs, [&dialog](const auto &ptr) { return ptr.get() == &dialog; });
      iter != dialogs.end()) {
    dialogs.erase(iter);
  }
}
DragNDropGroup &ImGuiInterface::createDragNDropGroup() { return dragNDropGroups.emplace_back(); }

FontManager &ImGuiInterface::getFontManager() { return fontManager; }

}// namespace pf::ui::ig