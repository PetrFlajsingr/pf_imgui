//
// Created by petr on 4/11/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_DETAILS_TOGGLEBUTTON_H
#define PF_IMGUI_SRC_PF_IMGUI_DETAILS_TOGGLEBUTTON_H

#include <imgui_internal.h>

namespace pf::ui::ig {

inline bool ToggleButton(const char *str_id, bool *v) {
  ImVec2 p = ImGui::GetCursorScreenPos();
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  float height = ImGui::GetFrameHeight();
  float width = height * 1.55f;
  float radius = height * 0.50f;
  ImGui::InvisibleButton(str_id, ImVec2(width, height));
  const auto wasClicked = ImGui::IsItemClicked();
  if (wasClicked) *v = !*v;

  float t = *v ? 1.0f : 0.0f;

  ImGuiContext &g = *GImGui;
  float ANIM_SPEED = 0.08f;
  if (g.LastActiveId == g.CurrentWindow->GetID(str_id))  // && g.LastActiveIdTimer < ANIM_SPEED)
  {
    float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
    t = *v ? (t_anim) : (1.0f - t_anim);
  }

  ImU32 col_bg;
  if (ImGui::IsItemHovered()) {
    auto col = ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive];
    col.x *= 1.1f;
    col.y *= 1.1f;
    col.z *= 1.1f;
    col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), col, t));
  } else {
    col_bg = ImGui::GetColorU32(
        ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive], t));
  }
  draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
  draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f,
                             IM_COL32(255, 255, 255, 255));
  return wasClicked;
}
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_SRC_PF_IMGUI_DETAILS_TOGGLEBUTTON_H
