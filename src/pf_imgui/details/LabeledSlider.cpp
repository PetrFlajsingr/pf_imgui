//
// Created by xflajs00 on 08.10.2022.
//

#include "LabeledSlider.h"
#include "DataTypeSize.h"
#include <cstring>
#include <imgui_internal.h>

namespace ImGui {
bool LabeledSliderScalar(const char *label, ImGuiDataType data_type, void *p_data, const void *p_min, const void *p_max,
                         const char *format, const char *componentLabel, ImU32 componentColor,
                         ImGuiSliderFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems) return false;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;
  const ImGuiID id = window->GetID(label);
  const float w = CalcItemWidth();

  const auto componentLabelSize = CalcTextSize(componentLabel, nullptr, false);

  const ImVec2 label_size = CalcTextSize(label, NULL, true);
  const ImRect frame_bb(window->DC.CursorPos + /*ADDED*/ ImVec2{componentLabelSize.x + 2 * style.FramePadding.x, 0.f},
                        window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
  const ImRect total_bb(frame_bb.Min - /*ADDED*/ ImVec2{componentLabelSize.x + 2 * style.FramePadding.x, 0.f},
                        frame_bb.Max
                            + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

  const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
  ItemSize(total_bb, style.FramePadding.y);
  if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

  // Default format string when passing NULL
  if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

  const bool hovered = ItemHoverable(frame_bb, id);
  bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
  if (!temp_input_is_active) {
    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool input_requested_by_tabbing =
        temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
    const bool clicked = (hovered && g.IO.MouseClicked[0]);
    const bool make_active =
        (input_requested_by_tabbing || clicked || g.NavActivateId == id || g.NavActivateInputId == id);
    if (make_active && temp_input_allowed)
      if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || g.NavActivateInputId == id)
        temp_input_is_active = true;

    if (make_active && !temp_input_is_active) {
      SetActiveID(id, window);
      SetFocusID(id, window);
      FocusWindow(window);
      g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }
  }

  if (temp_input_is_active) {
    // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
    const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
    return TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL,
                           is_clamp_input ? p_max : NULL);
  }

  // Draw frame
  const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive
                                          : hovered    ? ImGuiCol_FrameBgHovered
                                                       : ImGuiCol_FrameBg);
  RenderNavHighlight(frame_bb, id);
  RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

  // Slider behavior
  ImRect grab_bb;
  const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
  if (value_changed) MarkItemEdited(id);

  // ADDED display component label
  const auto componentLabelBB =
      ImRect{total_bb.Min, {total_bb.Min.x + componentLabelSize.x + 2 * style.FramePadding.x, total_bb.Max.y}};
  RenderFrame(componentLabelBB.Min, componentLabelBB.Max, componentColor, true,
              style.FrameRounding);
  RenderTextClipped(componentLabelBB.Min, componentLabelBB.Max, componentLabel,
                    componentLabel + std::strlen(componentLabel), nullptr, ImVec2(0.5f, 0.5f));

  // Render grab
  if (grab_bb.Max.x > grab_bb.Min.x)
    window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max,
                                    GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab),
                                    style.GrabRounding);

  // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
  char value_buf[64];
  const char *value_buf_end =
      value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
  if (g.LogEnabled) LogSetNextTextDecoration("{", "}");
  RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

  if (label_size.x > 0.0f)
    RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

  IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
  return value_changed;
}

bool LabeledSliderScalarN(const char *label, ImGuiDataType data_type, void *v, int components, const void *v_min,
                          const void *v_max, const char *format, const char **componentLabels,
                          const ImU32 *componentColors, ImGuiSliderFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems) return false;

  ImGuiContext &g = *GImGui;
  bool value_changed = false;
  BeginGroup();
  PushID(label);
  PushMultiItemsWidths(components, CalcItemWidth());
  size_t type_size = SizeForDataType(data_type);
  for (int i = 0; i < components; i++) {
    PushID(i);
    if (i > 0) SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |=
        LabeledSliderScalar("", data_type, v, v_min, v_max, format, componentLabels[i], componentColors[i], flags);
    PopID();
    PopItemWidth();
    v = (void *) ((char *) v + type_size);
  }
  PopID();

  const char *label_end = FindRenderedTextEnd(label);
  if (label != label_end) {
    SameLine(0, g.Style.ItemInnerSpacing.x);
    TextEx(label, label_end);
  }

  EndGroup();
  return value_changed;
}
}  // namespace ImGui