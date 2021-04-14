//
// Created by petr on 11/4/20.
//

#ifndef PF_IMGUI_ENUMS_H
#define PF_IMGUI_ENUMS_H

#include <cstdint>
#include <imgui.h>
#include <pf_common/enums.h>
#include <type_traits>

namespace pf::ui::ig {
/**
 * Enum for visibility of various types.
 */
enum class Visibility { Visible, Invisible };
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
  Normal,     /**< Typical button with label in the clickable area */
  Invisible,  /**< Invisible button */
  Small,      /**< Smaller version of a button */
  ArrowUp,    /**< Button without a label with an up aiming arrow */
  ArrowLeft,  /**< Button without a label with a left aiming arrow */
  ArrowRight, /**< Button without a label with a right aiming arrow */
  ArrowDown   /**< Button without a label with a down aiming arrow */
};
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
ENABLE_BIT_MASK_ENUM(TableBorder)
/**
 * Create ImGui flags for table API.
 * @param tableBorder type of rendered border
 * @param resizable resizable table
 * @param reorderable table may be ordered by moving header
 * @param sortable table may be sorted by clicking header
 * @param hideable
 * @return ImGUi flags
 */
inline ImGuiTableFlags createFlags(TableBorder tableBorder, bool resizable, bool reorderable, bool sortable,
                                   bool hideable) {
  auto result = ImGuiTableFlags{};
  if (is(tableBorder, TableBorder::HorizontalInner)) { result |= ImGuiTableFlags_BordersInnerH; }
  if (is(tableBorder, TableBorder::VerticalInner)) { result |= ImGuiTableFlags_BordersInnerV; }
  if (is(tableBorder, TableBorder::HorizontalOuter)) { result |= ImGuiTableFlags_BordersOuterH; }
  if (is(tableBorder, TableBorder::VerticalOuter)) { result |= ImGuiTableFlags_BordersOuterV; }
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
ENABLE_BIT_MASK_ENUM(Stretch)

/**
 * Enum for anchor type of AnchorLayout.
 */
// clang-format off
enum class Anchor : uint8_t {
  Top =                 0x1,
  Left =                0x2,
  Bottom =              0x4,
  Right =               0x8,
  TopLeft =             0x3,
  TopRight =            0x5,
  BottomLeft =          0x6,
  BottomRight =         0xC,
  TopBottomLeft =       0x7,
  TopBottomRight =      0xD,
  TopBottomLeftRight =  0xFF
};
// clang-format oon
ENABLE_BIT_MASK_ENUM(Anchor)
}// namespace pf::ui::ig

#endif//PF_IMGUI_ENUMS_H
