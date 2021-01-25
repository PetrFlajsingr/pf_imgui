//
// Created by petr on 11/4/20.
//

#ifndef PF_IMGUI_ENUMS_H
#define PF_IMGUI_ENUMS_H

#include <cstdint>
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

enum class Anchor : uint8_t { Top=0x1, Left=0x2, Bottom=0x4, Right=0x8,
                              TopLeft=0x3, TopRight=0x5, BottomLeft=0x6, BottomRight=0xC,
                              TopBottomLeft=0x7, TopBottomRight=0xD, TopBottomLeftRight=0xFF};

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
