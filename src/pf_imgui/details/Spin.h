//
// Created by petr on 5/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_DETAILS_SPIN_H
#define PF_IMGUI_SRC_PF_IMGUI_DETAILS_SPIN_H

#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui {
IMGUI_API inline bool SpinScaler(const char *label, ImGuiDataType data_type, void *data_ptr, const void *step,
                                 const void *step_fast, const char *format, ImGuiInputTextFlags flags) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems) return false;

  ImGuiContext &g = *GImGui;
  ImGuiStyle &style = g.Style;

  if (format == NULL) format = DataTypeGetInfo(data_type)->PrintFmt;

  char buf[64];
  DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, data_ptr, format);

  bool value_changed = false;
  if ((flags & (ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsScientific)) == 0)
    flags |= ImGuiInputTextFlags_CharsDecimal;
  flags |= ImGuiInputTextFlags_AutoSelectAll;
  flags |=
      ImGuiInputTextFlags_NoMarkEdited;// We call MarkItemEdited() ourselve by comparing the actual data rather than the string.

  if (step != NULL) {
    const float button_size = GetFrameHeight();

    BeginGroup();// The only purpose of the group here is to allow the caller to query item data e.g. IsItemActive()
    PushID(label);
    SetNextItemWidth(ImMax(1.0f, CalcItemWidth() - (button_size + style.ItemInnerSpacing.x) * 2));
    if (InputText("", buf, IM_ARRAYSIZE(buf),
                  flags))// PushId(label) + "" gives us the expected ID from outside point of view
      value_changed = DataTypeApplyFromText(buf, data_type, data_ptr, format);

    // Step buttons
    const ImVec2 backup_frame_padding = style.FramePadding;
    style.FramePadding.x = style.FramePadding.y;
    ImGuiButtonFlags button_flags = ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups;
    // FIXME if (flags & ImGuiInputTextFlags_ReadOnly) button_flags |= ImGuiButtonFlags_Disabled;
    SameLine(0, style.ItemInnerSpacing.x);

    // start diffs
    float frame_height = GetFrameHeight();
    float arrow_size = std::floor(frame_height * .45f);
    float arrow_spacing = frame_height - 2.0f * arrow_size;

    BeginGroup();
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{g.Style.ItemSpacing.x, arrow_spacing});

    // save/change font size to draw arrow buttons correctly
    float org_font_size = GetDrawListSharedData()->FontSize;
    GetDrawListSharedData()->FontSize = arrow_size;

    if (ArrowButtonEx("+", ImGuiDir_Up, ImVec2(arrow_size, arrow_size), button_flags)) {
      DataTypeApplyOp(data_type, '+', data_ptr, data_ptr, g.IO.KeyCtrl && step_fast ? step_fast : step);
      value_changed = true;
    }

    if (ArrowButtonEx("-", ImGuiDir_Down, ImVec2(arrow_size, arrow_size), button_flags)) {
      DataTypeApplyOp(data_type, '-', data_ptr, data_ptr, g.IO.KeyCtrl && step_fast ? step_fast : step);
      value_changed = true;
    }

    // restore font size
    GetDrawListSharedData()->FontSize = org_font_size;

    PopStyleVar(1);
    EndGroup();
    // end diffs

    const char *label_end = FindRenderedTextEnd(label);
    if (label != label_end) {
      SameLine(0, style.ItemInnerSpacing.x);
      TextEx(label, label_end);
    }
    style.FramePadding = backup_frame_padding;

    PopID();
    EndGroup();
  } else {
    if (InputText(label, buf, IM_ARRAYSIZE(buf), flags))
      value_changed = DataTypeApplyFromText(buf, data_type, data_ptr, format);
  }
  if (value_changed) MarkItemEdited(GImGui->LastItemData.ID);// TODO: check

  return value_changed;
}

IMGUI_API inline bool SpinInt(const char *label, int *v, int step = 1, int step_fast = 100,
                              ImGuiInputTextFlags flags = 0) {
  // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
  const char *format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
  return SpinScaler(label, ImGuiDataType_S32, (void *) v, (void *) (step > 0 ? &step : NULL),
                    (void *) (step_fast > 0 ? &step_fast : NULL), format, flags);
}

IMGUI_API inline bool SpinFloat(const char *label, float *v, float step = 0.0f, float step_fast = 0.0f,
                                const char *format = "%.3f", ImGuiInputTextFlags flags = 0) {
  flags |= ImGuiInputTextFlags_CharsScientific;
  return SpinScaler(label, ImGuiDataType_Float, (void *) v, (void *) (step > 0.0f ? &step : NULL),
                    (void *) (step_fast > 0.0f ? &step_fast : NULL), format, flags);
}

IMGUI_API inline bool SpinDouble(const char *label, double *v, double step = 0.0, double step_fast = 0.0,
                                 const char *format = "%.6f", ImGuiInputTextFlags flags = 0) {
  flags |= ImGuiInputTextFlags_CharsScientific;
  return SpinScaler(label, ImGuiDataType_Double, (void *) v, (void *) (step > 0.0 ? &step : NULL),
                    (void *) (step_fast > 0.0 ? &step_fast : NULL), format, flags);
}
};// namespace ImGui

#endif//PF_IMGUI_SRC_PF_IMGUI_DETAILS_SPIN_H
