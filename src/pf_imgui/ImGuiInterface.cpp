//
// Created by petr on 10/27/20.
//

#include <ImGuizmo.h>
#include <implot.h>
#include <pf_imgui/ImGuiInterface.h>
#include <pf_imgui/serialization.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/filter.hpp>
#include <utility>

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfig config)
    : Renderable("imgui_interface"), imguiContext(ImGui::CreateContext()), imPlotContext(ImPlot::CreateContext()),
      io(ImGui::GetIO()), fontManager(*this), notificationManager(), config(std::move(config.config)) {
  io.ConfigFlags = *config.flags;
  io.IniFilename = nullptr;
  ImGui::StyleColorsDark();
}

ImGuiInterface::~ImGuiInterface() {
  ImGui::DestroyContext(imguiContext);
  ImPlot::DestroyContext(imPlotContext);
}

ImGuiIO &ImGuiInterface::getIo() const { return io; }

AppMenuBar &ImGuiInterface::createOrGetMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<AppMenuBar>("app_menu_bar"); }
  return *menuBar;
}

bool ImGuiInterface::hasMenuBar() const { return menuBar != nullptr; }

void ImGuiInterface::removeMenuBar() { menuBar = nullptr; }

AppStatusBar &ImGuiInterface::createOrGetStatusBar() {
  if (!hasStatusBar()) { statusBar = std::make_unique<AppStatusBar>("status_bar"); }
  return *statusBar;
}

bool ImGuiInterface::hasStatusBar() const { return statusBar != nullptr; }

void ImGuiInterface::removeStatusBar() { statusBar = nullptr; }

const toml::table &ImGuiInterface::getConfig() const { return config; }

void ImGuiInterface::updateConfig() {
  std::ranges::for_each(windows, [this](const auto &window) {
    auto serialisedTree = serializeImGuiTree(*window);
    for (const auto &[key, value] : serialisedTree) { config.insert_or_assign(key, value); }
    if (menuBar != nullptr) {
      auto serialisedAppBar = serializeImGuiTree(*menuBar);
      for (const auto &[key, value] : serialisedAppBar) { config.insert_or_assign(key, value); }
    }
  });
  if (dialogManager.fileDialogBookmark.has_value()) {
    config.insert_or_assign("file_dialog_bookmark", *dialogManager.fileDialogBookmark);
  }
  std::ranges::for_each(radioGroups, [this](const auto &radioGroup) {
    if (radioGroup->isPersistent()) { config.insert_or_assign(radioGroup->getGroupName(), radioGroup->toToml()); }
  });
  //  if (io.WantSaveIniSettings) {
  io.WantSaveIniSettings = false;
  std::size_t iniSize;
  const auto imguiIniDataRaw = ImGui::SaveIniSettingsToMemory(&iniSize);
  std::string imguiIniData{imguiIniDataRaw, iniSize};
  config.insert_or_assign("imgui_ini", imguiIniData);
  // }
}

void ImGuiInterface::setStateFromConfig() {
  const auto serialiseSubtree = [this](Renderable &root) {
    traverseImGuiTree(root, [this](Renderable &renderable) {
      if (auto ptrSavable = dynamic_cast<Savable *>(&renderable); ptrSavable != nullptr && ptrSavable->isPersistent()) {
        if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr) {
          if (auto elemDataIter = config.find(ptrElement->getName()); elemDataIter != config.end()) {
            if (auto elemData = elemDataIter->second.as_table(); elemData != nullptr) {
              ptrSavable->setFromToml(*elemData);
            }
          }
        }
      }
    });
  };
  if (menuBar != nullptr) { serialiseSubtree(*menuBar); }
  std::ranges::for_each(windows, [&serialiseSubtree](auto &window) { serialiseSubtree(*window); });
  if (auto iter = config.find("file_dialog_bookmark"); iter != config.end()) {
    if (auto str = iter->second.as_string(); str != nullptr) { dialogManager.fileDialogBookmark = str->get(); }
  }
  std::ranges::for_each(radioGroups, [this](const auto &radioGroup) {
    if (auto iter = config.find(radioGroup->getGroupName()); iter != config.end() && radioGroup->isPersistent()) {
      if (auto data = iter->second.as_table(); data != nullptr) { radioGroup->setFromToml(*data); }
    }
  });
  if (auto imguiIniToml = config.find("imgui_ini"); imguiIniToml != config.end()) {
    if (auto imguiIni = imguiIniToml->second.as_string(); imguiIni != nullptr) {
      ImGui::LoadIniSettingsFromMemory(imguiIni->get().data(), imguiIni->get().size());
    }
  }
}

bool ImGuiInterface::isWindowHovered() const {
  setContext();
  return io.WantCaptureMouse;
}

bool ImGuiInterface::isKeyboardCaptured() const {
  setContext();
  return io.WantCaptureKeyboard;
}

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

CommandPaletteWindow &ImGuiInterface::createCommandPalette(const std::string &windowName) {
  return *commandPalettes.emplace_back(std::make_unique<CommandPaletteWindow>(windowName));
}

void ImGuiInterface::removePaletteWindow(const std::string &windowName) {
  auto remove = std::ranges::remove(commandPalettes, windowName, [](const auto &window) { return window->getName(); });
  commandPalettes.erase(remove.begin());  //-V539
}

void ImGuiInterface::removePaletteWindow(const CommandPaletteWindow &window) {
  auto remove = std::ranges::remove(commandPalettes, &window, &std::unique_ptr<CommandPaletteWindow>::get);
  commandPalettes.erase(remove.begin(), remove.end());  //-V539
}

DockBuilder &ImGuiInterface::createDockBuilder(DockSpace &dockSpace) {
  return *dockBuilders.emplace_back(std::unique_ptr<DockBuilder>{new DockBuilder{dockSpace}});  //-V824
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

std::optional<std::reference_wrapper<const Window>> ImGuiInterface::windowByName(const std::string &windowName) const {
  if (auto window = findIf(getWindows() | ranges::views::addressof,
                           [windowName](const auto &window) { return window->getName() == windowName; });
      window.has_value()) {
    return **window;
  } else {
    return std::nullopt;
  }
}

void ImGuiInterface::renderImpl() {
  ImGuizmo::BeginFrame();
  if (viewportGizmo != nullptr) { viewportGizmo->render(); }
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  if (hasMenuBar()) {
    if (backgroundDockingArea != nullptr) { backgroundDockingArea->leftTopMargin = ImVec2{0, ImGui::GetFrameHeight()}; }
    menuBar->render();
  } else {
    if (backgroundDockingArea != nullptr) { backgroundDockingArea->leftTopMargin = ImVec2{0, 0}; }
  }
  if (hasStatusBar()) {
    if (backgroundDockingArea != nullptr) {
      backgroundDockingArea->bottomRightMargin = ImVec2{0, ImGui::GetFrameHeight()};
    }
  } else {
    if (backgroundDockingArea != nullptr) { backgroundDockingArea->bottomRightMargin = ImVec2{0, 0}; }
  }
  if (backgroundDockingArea != nullptr) { backgroundDockingArea->render(); }
  std::ranges::for_each(windows, [](auto &window) { window->render(); });
  std::ranges::for_each(commandPalettes, [](auto &window) { window->render(); });
  std::ranges::for_each(dragNDropGroups, &DragNDropGroup::frame, &std::unique_ptr<DragNDropGroup>::get);
  std::ranges::for_each(radioGroups, &RadioGroup::frame, &std::unique_ptr<RadioGroup>::get);

  auto anyDockBuilderRun = false;
  std::ranges::for_each(dockBuilders | ranges::views::filter([](std::unique_ptr<DockBuilder> &builder) {
                          return builder->dockSpaceRef.isInitialised();
                        }),
                        [&anyDockBuilderRun](std::unique_ptr<DockBuilder> &builder) {
                          builder->run();
                          anyDockBuilderRun = true;
                        });
  if (anyDockBuilderRun) {
    const auto [rmBeg, rmEnd] = std::ranges::remove_if(
        dockBuilders, [](std::unique_ptr<DockBuilder> &builder) { return builder->dockSpaceRef.isInitialised(); });
    dockBuilders.erase(rmBeg, rmEnd);
  }
  if (statusBar != nullptr) { statusBar->render(); }
  dialogManager.renderDialogs();
  notificationManager.renderNotifications();
}

DragNDropGroup &ImGuiInterface::createDragNDropGroup() {
  return *dragNDropGroups.emplace_back(std::make_unique<DragNDropGroup>());
}

RadioGroup &ImGuiInterface::createRadioGroup(const std::string &groupName, Persistent persistent) {
  return *radioGroups.emplace_back(std::make_unique<RadioGroup>(groupName, std::vector<RadioButton *>{}, persistent));
}

BackgroundDockingArea &ImGuiInterface::createOrGetBackgroundDockingArea() {
  if (backgroundDockingArea == nullptr) {
    backgroundDockingArea = std::make_unique<BackgroundDockingArea>("background_docking_area");
  }
  return *backgroundDockingArea;
}

void ImGuiInterface::removeBackgroundDockingArea() { backgroundDockingArea = nullptr; }

Font ImGuiInterface::getGlobalFont() const { return globalFont; }

void ImGuiInterface::setGlobalFont(Font newFont) { globalFont = std::move(newFont); }

FontManager &ImGuiInterface::getFontManager() { return fontManager; }

const FontManager &ImGuiInterface::getFontManager() const { return fontManager; }

NotificationManager &ImGuiInterface::getNotificationManager() { return notificationManager; }

const NotificationManager &ImGuiInterface::getNotificationManager() const { return notificationManager; }

DialogManager &ImGuiInterface::getDialogManager() { return dialogManager; }

const DialogManager &ImGuiInterface::getDialogManager() const { return dialogManager; }

void ImGuiInterface::render() {
  setContext();
  if (shouldUpdateFontAtlas) {
    shouldUpdateFontAtlas = false;
    updateFonts();
  }
  newFrame_impl();
  ImGui::NewFrame();
  RAII endFrameRAII{[&] {
    ImGui::Render();
    renderDrawData_impl(ImGui::GetDrawData());
    if (getIo().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }
  }};
  if (getVisibility() == Visibility::Visible) {
    [[maybe_unused]] auto fontScoped = globalFont.applyScoped();
    if (getEnabled() == Enabled::No) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
  }
}

void ImGuiInterface::setContext() const { ImGui::SetCurrentContext(imguiContext); }

void ImGuiInterface::removeViewportGizmo() { viewportGizmo = nullptr; }

}  // namespace pf::ui::ig
