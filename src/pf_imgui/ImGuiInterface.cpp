//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "elements/Window.h"
#include "serialization.h"
#include <imgui_internal.h>
#include <implot.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig)
    : io(baseInit(flags)), config(std::move(tomlConfig)) {}

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

Dialog &ImGuiInterface::createDialog(const std::string &elementName, const std::string &caption, Modal modal) {
  auto dialog = std::make_unique<Dialog>(dialogContainer, elementName, caption, modal);
  const auto ptr = dialog.get();
  dialogContainer.addChild(std::move(dialog));
  return *ptr;
}

AppMenuBar &ImGuiInterface::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<AppMenuBar>("app_menu_bar"); }
  return *menuBar;
}
bool ImGuiInterface::hasMenuBar() const { return menuBar != nullptr; }
const toml::table &ImGuiInterface::getConfig() const { return config; }

void ImGuiInterface::updateConfig() {
  config = serializeImGuiTree(*this);
}

void ImGuiInterface::setStateFromConfig() {
  traverseImGuiTree(*this, [this](Renderable &renderable) {
    if (auto ptrSavable = dynamic_cast<Savable *>(&renderable); ptrSavable != nullptr) {
      if (auto ptrElement = dynamic_cast<Element *>(&renderable); ptrElement != nullptr) {
        if (config.contains(ptrElement->getName())) { ptrSavable->unserialize(*config[ptrElement->getName()].as_table()); }
      }
    }
  });
}
void ImGuiInterface::renderFileDialogs() {
  std::ranges::for_each(fileDialogs, [](auto &dialog) { dialog.render(); });
  if (const auto iter = std::ranges::find_if(fileDialogs, [](auto &dialog) { return dialog.isDone(); });
      iter != fileDialogs.end()) {
    fileDialogs.erase(iter);
  }
}
bool ImGuiInterface::isWindowHovered() const { return io.WantCaptureMouse; }

bool ImGuiInterface::isKeyboardCaptured() const { return io.WantCaptureKeyboard; }

ImVec2 ImGuiInterface::getCursorPosition() const { return ImGui::GetCursorScreenPos(); }

void ImGuiInterface::setCursorPosition(const ImVec2 &position) { ImGui::SetCursorScreenPos(position); }

void ImGuiInterface::render() {
  std::ranges::for_each(windows, [](auto &window) { window->render(); });
}

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

}// namespace pf::ui::ig