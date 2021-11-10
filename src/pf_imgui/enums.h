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
#include <imgui_notify.h>
#include <pf_common/enums.h>
#include <type_traits>

// TODO: add convenience operators for some enums
namespace pf::ui::ig {
enum class MouseButton {
  Left = ImGuiMouseButton_::ImGuiMouseButton_Left,
  Right = ImGuiMouseButton_::ImGuiMouseButton_Right,
  Middle = ImGuiMouseButton_::ImGuiMouseButton_Middle
};
enum class NotificationType {
  Success = ImGuiToastType_Success,
  Warning = ImGuiToastType_Warning,
  Error = ImGuiToastType_Error,
  Info = ImGuiToastType_Info
};
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
* Enum for button type, which changes the shape of a button.
*/
enum class ButtonType {
  Normal,     /*!< Typical button with label in the clickable area */
  Small,      /*!< Smaller version of a button */
  ArrowUp,    /*!< Button without a label with an up aiming arrow */
  ArrowLeft,  /*!< Button without a label with a left aiming arrow */
  ArrowRight, /*!< Button without a label with a right aiming arrow */
  ArrowDown   /*!< Button without a label with a down aiming arrow */
};
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
// clang-format off
enum class TableBorder : uint16_t {
 None =            0x00,
 HorizontalInner = 0x01,
 VerticalInner =   0x02,
 Inner =           0x03,
 HorizontalOuter = 0x04,
 VerticalOuter =   0x08,
 Outer =           0x0C,
 Full =            0x0F
};
// clang-format on

/**
* Create ImGui flags for table API.
* @param tableBorder type of rendered border
* @param resizable resizable table
* @param reorderable table may be ordered by moving header
* @param sortable table may be sorted by clicking header
* @param hideable
* @return ImGUi flags
*/
inline ImGuiTableFlags createFlags(const Flags<TableBorder>& tableBorder, bool resizable, bool reorderable, bool sortable,
                                   bool hideable) {
  auto result = ImGuiTableFlags{};
  if (tableBorder.is(TableBorder::HorizontalInner)) { result |= ImGuiTableFlags_BordersInnerH; }
  if (tableBorder.is(TableBorder::VerticalInner)) { result |= ImGuiTableFlags_BordersInnerV; }
  if (tableBorder.is(TableBorder::HorizontalOuter)) { result |= ImGuiTableFlags_BordersOuterH; }
  if (tableBorder.is(TableBorder::VerticalOuter)) { result |= ImGuiTableFlags_BordersOuterV; }
  if (resizable) { result |= ImGuiTableFlags_Resizable; }
  if (reorderable) { result |= ImGuiTableFlags_Reorderable; }
  if (sortable) { result |= ImGuiTableFlags_Sortable; }
  if (hideable) { result |= ImGuiTableFlags_Hideable; }
  return result;
}
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
 LeftRight =           Left | Right,
 TopLeft =             Top | Left,
 TopRight =            Top | Right,
 TopBottom =           Top | Bottom,
 BottomLeft =          Bottom | Left,
 BottomRight =         Bottom | Right,
 TopLeftRight =        Top | Left | Right,
 BottomLeftRight =     Bottom | Left | Right,
 TopBottomLeft =       Top | Left | Bottom,
 TopBottomRight =      Top | Right | Bottom,
 TopBottomLeftRight =  Top | Left | Bottom | Right
};
// clang-format on

}// namespace pf::ui::ig

#endif//PF_IMGUI_ENUMS_H
