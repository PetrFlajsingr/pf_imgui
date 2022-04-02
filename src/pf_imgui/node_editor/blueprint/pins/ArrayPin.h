//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_ARRAYPIN_H
#define PF_IMGUI_BLUEPRINT_ARRAYPIN_H

#include "Pin.h"
#include "PinDraw.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

template<typename T>
class ArrayPin : public Pin {
 public:
  using ValueType = std::vector<T>;

  ArrayPin(const std::string &name, const std::string &label, Color color) : Pin(name, label, color) {}

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override {
    if (!Pin::acceptsLinkWith(other)) { return false; }
    return dynamic_cast<ArrayPin *>(&other) != nullptr;
  }

 protected:
  void renderIcon() override {
    ImVec2 iconSize{16, 16};
    if (ImGui::IsRectVisible(iconSize)) {
      auto cursorPos = ImGui::GetCursorScreenPos();
      auto drawList = ImGui::GetWindowDrawList();

      drawPin3by3Quads(drawList, cursorPos + ImVec2{1, 3}, getColor(), hasAnyValidLinks());
    }
    ImGui::Dummy(iconSize);
  }
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_ARRAYPIN_H
