//
// Created by petr on 6/18/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_STYLES_ENUMS_H
#define PF_IMGUI_SRC_PF_IMGUI_STYLES_ENUMS_H

#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/algorithms.h>

namespace pf::ui::ig::style {

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
};

enum class Style : int {
  //float:
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
  //ImVec2
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

constexpr bool isFloatStyle(Style style) {
  return isIn(style,
              std::array<Style, 15>{Style::Alpha, Style::WindowRounding, Style::WindowBorderSize, Style::ChildRounding,
                          Style::ChildBorderSize, Style::PopupRounding, Style::PopupBorderSize, Style::FrameRounding,
                          Style::FrameBorderSize, Style::IndentSpacing, Style::ScrollbarSize, Style::ScrollbarRounding,
                          Style::GrabMinSize, Style::GrabRounding, Style::TabRounding});
}

}// namespace pf::ui::ig::style

#endif//PF_IMGUI_SRC_PF_IMGUI_STYLES_ENUMS_H
