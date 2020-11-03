//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "serialization.h"

namespace pf::ui::ig {

ImGuiInterface::ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig)
    : Container("Main"), io(baseInit(flags)), config(std::move(tomlConfig)) {}
ImGuiIO &ImGuiInterface::baseInit(ImGuiConfigFlags flags) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto &imguiIo = ImGui::GetIO();
  imguiIo.ConfigFlags |= flags;
  ImGui::StyleColorsDark();
  return imguiIo;
}

ImGuiIO &ImGuiInterface::getIo() const { return io; }
std::shared_ptr<Dialog> ImGuiInterface::createDialog(const std::string &elementName,
                                                          const std::string &caption, Modal modal) {
  auto result = std::make_shared<Dialog>(*this, elementName, caption, modal);
  addChild(result);
  return result;
}

ImGuiAppMenuBar &ImGuiInterface::getMenuBar() {
  if (!menuBar.has_value()) { menuBar = ImGuiAppMenuBar("app_menu_bar"); }
  return *menuBar;
}
bool ImGuiInterface::hasMenuBar() const { return menuBar.has_value(); }
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

}// namespace pf::ui