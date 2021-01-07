//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "serialization.h"
#include <implot.h>

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig)
    : Container("Main"), io(baseInit(flags)), config(std::move(tomlConfig)) {}

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
  auto dialog = std::make_unique<Dialog>(*this, elementName, caption, modal);
  const auto ptr = dialog.get();
  addChild(std::move(dialog));
  return *ptr;
}

AppMenuBar &ImGuiInterface::getMenuBar() {
  if (menuBar == nullptr) { menuBar = std::make_unique<AppMenuBar>("app_menu_bar"); }
  return *menuBar;
}
bool ImGuiInterface::hasMenuBar() const { return menuBar != nullptr; }
const toml::table &ImGuiInterface::getConfig() const { return config; }

void ImGuiInterface::updateConfig() { config = serializeImGuiTree(*this); }

void ImGuiInterface::setStateFromConfig() {
  traverseImGuiTree(*this, [this](Element &element) {
    if (auto ptrSavable = dynamic_cast<SavableElement *>(&element); ptrSavable != nullptr) {
      if (config.contains(ptrSavable->getName())) {
        ptrSavable->unserialize(*config[ptrSavable->getName()].as_table());
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

}// namespace pf::ui::ig