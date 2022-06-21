//
// Created by Petr on 6/17/2022.
//

#ifndef PF_IMGUI_STYLE_COMMON_H
#define PF_IMGUI_STYLE_COMMON_H

#include <imgui.h>
#include <optional>
#include <pf_common/algorithms.h>
#include <pf_imgui/Color.h>

namespace pf::ui::ig {

enum class ColorOf : int {
  Text = ImGuiCol_Text,
  TextDisabled = ImGuiCol_TextDisabled,
  WindowBackground = ImGuiCol_WindowBg,
  ChildBackground = ImGuiCol_ChildBg,
  PopupBackground = ImGuiCol_PopupBg,
  Border = ImGuiCol_Border,
  BorderShadow = ImGuiCol_BorderShadow,
  FrameBackground = ImGuiCol_FrameBg,
  FrameBackgroundHovered = ImGuiCol_FrameBgHovered,
  FrameBackgroundActive = ImGuiCol_FrameBgActive,
  TitleBackground = ImGuiCol_TitleBg,
  TitleBackgroundActive = ImGuiCol_TitleBgActive,
  TitleBackgroundCollapsed = ImGuiCol_TitleBgCollapsed,
  MenuBarBackground = ImGuiCol_MenuBarBg,
  ScrollbarBackground = ImGuiCol_ScrollbarBg,
  ScrollbarGrab = ImGuiCol_ScrollbarGrab,
  ScrollbarGrabHovered = ImGuiCol_ScrollbarGrabHovered,
  ScrollbarGrabActive = ImGuiCol_ScrollbarGrabActive,
  CheckMark = ImGuiCol_CheckMark,
  SliderGrab = ImGuiCol_SliderGrab,
  SliderGrabActive = ImGuiCol_SliderGrabActive,
  Button = ImGuiCol_Button,
  ButtonHovered = ImGuiCol_ButtonHovered,
  ButtonActive = ImGuiCol_ButtonActive,
  Header = ImGuiCol_Header,
  HeaderHovered = ImGuiCol_HeaderHovered,
  HeaderActive = ImGuiCol_HeaderActive,
  Separator = ImGuiCol_Separator,
  SeparatorHovered = ImGuiCol_SeparatorHovered,
  SeparatorActive = ImGuiCol_SeparatorActive,
  ResizeGrip = ImGuiCol_ResizeGrip,
  ResizeGripHovered = ImGuiCol_ResizeGripHovered,
  ResizeGripActive = ImGuiCol_ResizeGripActive,
  Tab = ImGuiCol_Tab,
  TabHovered = ImGuiCol_TabHovered,
  TabActive = ImGuiCol_TabActive,
  TabUnfocused = ImGuiCol_TabUnfocused,
  TabUnfocusedActive = ImGuiCol_TabUnfocusedActive,
  PlotLines = ImGuiCol_PlotLines,
  PlotLinesHovered = ImGuiCol_PlotLinesHovered,
  PlotHistogram = ImGuiCol_PlotHistogram,
  PlotHistogramHovered = ImGuiCol_PlotHistogramHovered,
  TableHeaderBackground = ImGuiCol_TableHeaderBg,
  TableBorderStrong = ImGuiCol_TableBorderStrong,
  TableBorderLight = ImGuiCol_TableBorderLight,
  TableRowBackground = ImGuiCol_TableRowBg,
  TableRowBackgroundAlt = ImGuiCol_TableRowBgAlt,
  TextSelectedBackground = ImGuiCol_TextSelectedBg,
  DragDropTarget = ImGuiCol_DragDropTarget,
  NavHighlight = ImGuiCol_NavHighlight,
  NavWindowingHighlight = ImGuiCol_NavWindowingHighlight,
  NavWindowingDimBackground = ImGuiCol_NavWindowingDimBg,
  ModalWindowDimBackground = ImGuiCol_ModalWindowDimBg,
  DockingPreview = ImGuiCol_DockingPreview,
  DockingBackground = ImGuiCol_DockingEmptyBg,
};

enum class StyleOf : int {
  // float:
  Alpha = ImGuiStyleVar_Alpha,
  WindowRounding = ImGuiStyleVar_WindowRounding,
  WindowBorderSize = ImGuiStyleVar_WindowBorderSize,
  ChildRounding = ImGuiStyleVar_ChildRounding,
  ChildBorderSize = ImGuiStyleVar_ChildBorderSize,
  PopupRounding = ImGuiStyleVar_PopupRounding,
  PopupBorderSize = ImGuiStyleVar_PopupBorderSize,
  FrameRounding = ImGuiStyleVar_FrameRounding,
  FrameBorderSize = ImGuiStyleVar_FrameBorderSize,
  IndentSpacing = ImGuiStyleVar_IndentSpacing,
  ScrollbarSize = ImGuiStyleVar_ScrollbarSize,
  ScrollbarRounding = ImGuiStyleVar_ScrollbarRounding,
  GrabMinSize = ImGuiStyleVar_GrabMinSize,
  GrabRounding = ImGuiStyleVar_GrabRounding,
  TabRounding = ImGuiStyleVar_TabRounding,
  // ImVec2
  WindowPadding = ImGuiStyleVar_WindowPadding,
  WindowMinSize = ImGuiStyleVar_WindowMinSize,
  WindowTitleAlign = ImGuiStyleVar_WindowTitleAlign,
  FramePadding = ImGuiStyleVar_FramePadding,
  ItemSpacing = ImGuiStyleVar_ItemSpacing,
  ItemInnerSpacing = ImGuiStyleVar_ItemInnerSpacing,
  CellPadding = ImGuiStyleVar_CellPadding,
  ButtonTextAlign = ImGuiStyleVar_ButtonTextAlign,
  SelectableTextAlign = ImGuiStyleVar_SelectableTextAlign,
};

namespace details {
template<ColorOf ColorType>
using ColorOfAsColor = Color;

template<ColorOf ColorType>
using ColorOfAsOptionalColor = std::optional<ColorOfAsColor<ColorType>>;

constexpr bool isFloatStyle(StyleOf style) {
  return isIn(style,
              std::array<StyleOf, 15>{StyleOf::Alpha, StyleOf::WindowRounding, StyleOf::WindowBorderSize,
                                      StyleOf::ChildRounding, StyleOf::ChildBorderSize, StyleOf::PopupRounding,
                                      StyleOf::PopupBorderSize, StyleOf::FrameRounding, StyleOf::FrameBorderSize,
                                      StyleOf::IndentSpacing, StyleOf::ScrollbarSize, StyleOf::ScrollbarRounding,
                                      StyleOf::GrabMinSize, StyleOf::GrabRounding, StyleOf::TabRounding});
}

template<StyleOf Style>
using TypeForStyle = std::conditional_t<isFloatStyle(Style), float, ImVec2>;

template<StyleOf Style>
using OptionalTypeForStyle = std::optional<TypeForStyle<Style>>;

}  // namespace details

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_STYLE_COMMON_H
