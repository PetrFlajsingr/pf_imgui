//
// Created by Petr on 6/23/2022.
//

#ifndef PF_IMGUI_ELEMENTS_DETAILS_TEXTUTILS_H
#define PF_IMGUI_ELEMENTS_DETAILS_TEXTUTILS_H

#include <imgui_internal.h>
#include <pf_imgui/common/Color.h>

namespace pf::ui::ig {

inline void drawTextBackground(const char *str, Color color, bool textWrapped, bool applyPadding) {
  const auto maxWidth = ImGui::GetContentRegionAvail().x;
  const auto textSize = ImGui::CalcTextSize(str, nullptr, false, textWrapped ? maxWidth : -1.f);
  ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(),
                                            ImGui::GetCursorScreenPos() + ImVec2{maxWidth, textSize.y}
                                                + (applyPadding ? ImGui::GetStyle().FramePadding * 2 : ImVec2{0, 0}),
                                            static_cast<ImU32>(color));
}

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_DETAILS_TEXTUTILS_H
