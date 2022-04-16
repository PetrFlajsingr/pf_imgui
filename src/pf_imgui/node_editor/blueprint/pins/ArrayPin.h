//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_ARRAYPIN_H
#define PF_IMGUI_BLUEPRINT_ARRAYPIN_H

#include "Pin.h"
#include "PinDraw.h"
#include <imgui_internal.h>
#include <pf_imgui/node_editor/blueprint/common.h>

namespace pf::ui::ig::bp {

template<typename T>
class ArrayPin : public Pin {
 public:
  using ValueType = std::vector<T>;
  PF_IMGUI_BLUEPRINT_PIN_ID(ArrayPin)

  ArrayPin(const std::string &name, const std::string &label, Color color) : Pin(name, label, color) {}

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override {
    if (!Pin::acceptsLinkWith(other)) { return false; }
    return dynamic_cast<ArrayPin *>(&other) != nullptr;
  }

  [[nodiscard]] static std::unique_ptr<ArrayPin> ConstructFromToml(ig::Node *parent, const toml::table &src) {
    auto result = std::make_unique<ArrayPin>("", "", Color::White);
    result->parent = parent;
    result->setFromToml(src);
    return result;
  }

 protected:
  void renderIcon() override {
    ImVec2 iconSize{16, 16};
    if (ImGui::IsRectVisible(iconSize)) {
      auto cursorPos = ImGui::GetCursorScreenPos();
      auto drawList = ImGui::GetWindowDrawList();

      drawPin3by3Quads(drawList, cursorPos + ImVec2{1, 3}, static_cast<ImVec4>(getColor()), hasAnyValidLinks());
    }
    ImGui::Dummy(iconSize);
  }
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_ARRAYPIN_H
