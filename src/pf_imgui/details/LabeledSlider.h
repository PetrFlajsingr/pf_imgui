//
// Created by xflajs00 on 08.10.2022.
//

#ifndef PF_IMGUI_DETAILS_LABELEDSLIDER_H
#define PF_IMGUI_DETAILS_LABELEDSLIDER_H

#include <imgui.h>

namespace ImGui {

bool LabeledSliderScalar(const char *label, ImGuiDataType data_type, void *p_data, const void *p_min, const void *p_max, const char *format,
                         const char *componentLabel, ImU32 componentColor, ImGuiSliderFlags flags);

bool LabeledSliderScalarN(const char *label, ImGuiDataType data_type, void *v, int components, const void *v_min, const void *v_max,
                          const char *format, const char **componentLabels, const ImU32 *componentColors, ImGuiSliderFlags flags);

}  // namespace ImGui

#endif