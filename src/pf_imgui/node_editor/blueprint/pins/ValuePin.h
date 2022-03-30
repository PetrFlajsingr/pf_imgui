//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_VALUEPIN_H
#define IMGUI_EXPERIMENTS_VALUEPIN_H

#include "Pin.h"
#include "PinDraw.h"
#include <string>

namespace pf::ui::ig::bp {

template<typename T>
class ValuePin : public Pin {
 public:
  using ValueType = T;

  ValuePin(const std::string &name, const std::string &label, const ImColor &color) : Pin(name, label, color) {}

 protected:
  void renderIcon() override {
    const static auto renderAreaSize = ImVec2{16, 16};
    if (ImGui::IsRectVisible(renderAreaSize)) {
      auto cursorPos = ImGui::GetCursorScreenPos();
      auto drawList = ImGui::GetWindowDrawList();

      const auto circleCenter = cursorPos + ImVec2{6, 8};

      drawPinCircle(drawList, circleCenter, getColor(), hasAnyValidLinks());
      drawPinArrow(drawList, circleCenter + ImVec2{6, 0}, getColor());
    }
    ImGui::Dummy(renderAreaSize);
  }

 public:
  bool acceptsLinkWith(ig::Pin &other) const override {
    if (!Pin::acceptsLinkWith(other)) { return false; }
    return dynamic_cast<ValuePin *>(&other) != nullptr;
  }
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_VALUEPIN_H
