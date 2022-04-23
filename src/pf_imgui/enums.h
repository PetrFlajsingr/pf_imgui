/**
* @file enums.h
* @brief Library's enums.
* @author Petr Flajšingr
* @date 4.11.20
*/

#ifndef PF_IMGUI_ENUMS_H
#define PF_IMGUI_ENUMS_H

#include <cstdint>
#include <imgui.h>
#include <pf_common/enums.h>
#include <type_traits>

// TODO: add convenience operators for some enums
namespace pf::ui::ig {
enum class MouseButton {
  Left = ImGuiMouseButton_::ImGuiMouseButton_Left,
  Right = ImGuiMouseButton_::ImGuiMouseButton_Right,
  Middle = ImGuiMouseButton_::ImGuiMouseButton_Middle
};

enum class Direction { Left = ImGuiDir_Left, Right = ImGuiDir_Right, Up = ImGuiDir_Up, Down = ImGuiDir_Down };

enum class HorizontalDirection { Left = ImGuiDir_Left, Right = ImGuiDir_Right };

enum class VerticalDirection { Up = ImGuiDir_Up, Down = ImGuiDir_Down };

enum class HorizontalAlign { Up, Middle, Down };

enum class VerticalAlign { Left, Middle, Right };

/**
* Enum for selection.
*/
enum class Selected { Yes, No };
/**
* Enum for visibility of various types.
*/
enum class Visibility { Visible, Invisible };
inline Visibility operator!(Visibility visibility) {
  return visibility == Visibility::Visible ? Visibility::Invisible : Visibility::Visible;
}
/**
* Enum for modality of dialogs.
*/
enum class Modal { Yes, No };
/**
* Enum for input type of text based elements.
*/
enum class TextInputType { SingleLine, MultiLine };
/**
* Enum for plot type of SimplePlot.
*/
enum class PlotType { Lines, Histogram };
/**
* Enable/disable repeatable behavior.
*/
enum class Repeatable { Yes, No };
/**
* Enum for marking persistence of Savable elements.
*/
enum class Persistent { Yes, No };
/**
* Enum for types of ColorChooser.
*/
enum class ColorChooserType { Edit, Picker };
enum class ColorChooserFormat { RGB, RGBA };
/**
* Enum for StackLayout direction.
*/
enum class LayoutDirection { LeftToRight, TopToBottom };
/**
* Enum for FileDialog selection type.
*/
enum class FileType { File, Directory };
/**
* Enum for marking elements as a button.
*/
enum class IsButton { Yes, No };
/**
* Enum for Collapsible.
*/
enum class AllowCollapse { Yes, No };
/**
* Enum for marking border render of layouts.
*/
enum class ShowBorder { Yes, No };
/**
* Enum for selection of which borders to render in a table. Functions as a bit mask.
*/
enum class TableBorder : int {
  None = 0x00,
  HorizontalInner = ImGuiTableFlags_BordersInnerH,
  VerticalInner = ImGuiTableFlags_BordersInnerV,
  Inner = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV,
  HorizontalOuter = ImGuiTableFlags_BordersOuterH,
  VerticalOuter = ImGuiTableFlags_BordersOuterV,
  Outer = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV,
  Full = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterH
      | ImGuiTableFlags_BordersOuterV
};
/**
* Enum for options of interaction with a table
*/
enum class TableOptions : int {
  None = 0,
  Resizable = ImGuiTableFlags_Resizable,
  Reorderable = ImGuiTableFlags_Reorderable,
  Sortable = ImGuiTableFlags_Sortable,
  Hideable = ImGuiTableFlags_Hideable
};

/**
* Enum for stretch type of StretchLayout.
*/
enum class Stretch : uint8_t { Width = 0x1, Height = 0x2, All = 0x3 };

/**
* Enum for anchor type of AnchorLayout.
*/
// clang-format off
enum class Anchor : uint8_t {
  Top =                 0x1,
  Left =                0x2,
  Bottom =              0x4,
  Right =               0x8,
  LeftRight =           Left   | Right,
  TopLeft =             Top    | Left,
  TopRight =            Top    | Right,
  TopBottom =           Top    | Bottom,
  BottomLeft =          Bottom | Left,
  BottomRight =         Bottom | Right,
  TopLeftRight =        Top    | Left  | Right,
  BottomLeftRight =     Bottom | Left  | Right,
  TopBottomLeft =       Top    | Left  | Bottom,
  TopBottomRight =      Top    | Right | Bottom,
  TopBottomLeftRight =  Top    | Left  | Bottom | Right
};
// clang-format on

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ENUMS_H
