//
// Created by xflajs00 on 08.10.2022.
//

#ifndef PF_IMGUI_DETAILS_LABELEDDRAG_H
#define PF_IMGUI_DETAILS_LABELEDDRAG_H

#include <imgui.h>

namespace ImGui {
bool LabeledDragScalar(const char *label, ImGuiDataType data_type, void *p_data, float v_speed, const void *p_min,
                       const void *p_max, const char *format, const char *componentLabel, ImColor componentColor,
                       ImGuiSliderFlags flags);

bool LabeledDragScalarN(const char *label, ImGuiDataType data_type, void *p_data, int components, float v_speed,
                        const void *p_min, const void *p_max, const char *format, const char **componentLabels,
                        const ImColor *componentColors, ImGuiSliderFlags flags);

}  // namespace ImGui

#endif