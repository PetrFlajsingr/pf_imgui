//
// Created by petr on 10/27/20.
//

#include "ImGuiInterface.h"
#include "serialization.h"
#include <imgui_internal.h>
#include <implot.h>
#include <pf_common/RAII.h>
#include <utility>

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
    if (auto ptrSavable = dynamic_cast<Savable *>(&element); ptrSavable != nullptr) {
      if (config.contains(element.getName())) {
        ptrSavable->unserialize(*config[element.getName()].as_table());
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
  if (getVisibility() == Visibility::Visible) {
    if (getEnabled() == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      auto raiiEnabled = pf::RAII([] {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      });
      renderImpl();
    } else {
      renderImpl();
    }
  }
}

}// namespace pf::ui::ig