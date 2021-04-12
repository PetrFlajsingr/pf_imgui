//
// Created by petr on 11/4/20.
//

#ifndef PF_IMGUI_ENUMS_H
#define PF_IMGUI_ENUMS_H

#include <cstdint>
#include <imgui.h>
#include <type_traits>

namespace pf::ui::ig {

enum class Visibility { Visible, Invisible };

enum class Modal { Yes, No };

enum class TextInputType { SingleLine, MultiLine };

enum class PlotType { Lines, Histogram };

enum class ButtonType { Normal, Invisible, Small, ArrowUp, ArrowLeft, ArrowRight, ArrowDown };

enum class Persistent { Yes, No };

enum class ColorChooserType { Edit, Picker };

enum class LayoutDirection { LeftToRight, TopToBottom };

enum class FileType { File, Directory };

enum class IsButton { Yes, No };

enum class AllowCollapse { Yes, No };

enum class ShowBorder { Yes, No };

enum class TableBorder : uint16_t {
  None = 0b0,
  HorizontalInner = 0b1,
  VerticalInner = 0b10,
  Inner = 0b11,
  HorizontalOuter = 0b100,
  VerticalOuter = 0b1000,
  Outer = 0b1100,
  Full = 0b1111
};

inline bool is(TableBorder lhs, TableBorder rhs) {
  using T = std::underlying_type_t<TableBorder>;
  return static_cast<T>(lhs) & static_cast<T>(rhs);
}

inline ImGuiTableFlags createFlags(TableBorder tableBorder, bool resizable, bool reorderable, bool sortable, bool hideable) {
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

enum class Stretch : uint8_t { Width = 0x1, Height = 0x2, All = 0x3 };

inline Stretch operator|(Stretch lhs, Stretch rhs) {
  using T = std::underlying_type_t<Stretch>;
  return static_cast<Stretch>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
inline Stretch &operator|=(Stretch &lhs, Stretch rhs) {
  lhs = lhs | rhs;
  return lhs;
}
inline bool is(Stretch self, Stretch other) {
  using T = std::underlying_type_t<Stretch>;
  return (static_cast<T>(self) & static_cast<T>(other)) != 0;
}
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
inline Anchor operator|(Anchor lhs, Anchor rhs) {
  using T = std::underlying_type_t<Anchor>;
  return static_cast<Anchor>(static_cast<T>(lhs) | static_cast<T>(rhs));
}
inline Anchor &operator|=(Anchor &lhs, Anchor rhs) {
  lhs = lhs | rhs;
  return lhs;
}
inline bool is(Anchor self, Anchor other) {
  using T = std::underlying_type_t<Anchor>;
  return (static_cast<T>(self) & static_cast<T>(other)) != 0;
}
}// namespace pf::ui::ig

#endif//PF_IMGUI_ENUMS_H
