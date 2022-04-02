//
// Created by xflajs00 on 29.03.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_VALUEPIN_H
#define PF_IMGUI_BLUEPRINT_VALUEPIN_H

#include "Pin.h"
#include "PinDraw.h"
#include <string>

namespace pf::ui::ig::bp {

template<typename T>
class ValuePin : public Pin {
 public:
  using ValueType = T;

  ValuePin(const std::string &name, const std::string &label, Color color) : Pin(name, label, color) {}

  [[nodiscard]] bool acceptsLinkWith(ig::Pin &other) const override {
    if (!Pin::acceptsLinkWith(other)) { return false; }
    return dynamic_cast<ValuePin *>(&other) != nullptr;
  }

 protected:
  void renderIcon() override {
    const static auto renderAreaSize = ImVec2{16, 16};
    if (ImGui::IsRectVisible(renderAreaSize)) {
      auto cursorPos = ImGui::GetCursorScreenPos();
      auto drawList = ImGui::GetWindowDrawList();

      const auto circleCenter = cursorPos + ImVec2{6, 8};

      drawPinCircle(drawList, circleCenter, static_cast<ImVec4>(getColor()), hasAnyValidLinks());
      drawPinArrow(drawList, circleCenter + ImVec2{6, 0}, static_cast<ImVec4>(getColor()));
    }
    ImGui::Dummy(renderAreaSize);
  }
};

}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_VALUEPIN_H
