/**
 * @file ColorPalette.h
 * @brief ColorPalette for element styling.
 * @author Petr Flajšingr
 * @date 17.6.22
 */

#ifndef PF_IMGUI_STYLE_COLORPALETTE_H
#define PF_IMGUI_STYLE_COLORPALETTE_H

#include <imgui.h>
#include <pf_common/RAII.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/meta.h>
#include <pf_common/tuple.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Color.h>
#include <pf_imgui/style/common.h>

namespace pf::ui::ig {

/**
 * @brief Color palette for imgui
 * @tparam SupportedColorTypes types used in this palette
 */
template<ColorOf... SupportedColorTypes>
class PF_IMGUI_EXPORT ColorPalette {
 public:
  /**
   * Set color for given type.
   * @tparam ColorType type to set color for
   */
  template<ColorOf ColorType>
    requires(OneOfValues_v<ColorType, SupportedColorTypes...>)
  void set(Color color) {
    modified = true;
    std::get<indexInVarArgList<ColorType, SupportedColorTypes...>()>(colorValues) = color;
  }

  template<ColorOf ColorType>
  [[nodiscard]] std::optional<Color> get() const {
    return std::get<indexInVarArgList<ColorType, SupportedColorTypes...>()>(colorValues);
  }

  /**
   * Reset color to default value.
   * @tparam ColorType color to reset
   */
  template<ColorOf ColorType>
  void unset() {
    std::get<indexInVarArgList<ColorType, SupportedColorTypes...>()>(colorValues) = std::nullopt;
  }

  /**
   * Reset all colors to default values.
   */
  void clear() {
    iterateTuple([](auto &value) { value = std::nullopt; }, colorValues);
  }

  /**
   * Add color options on imgui stack and return an instance of RAII which resets it.
   * @return RAII resetting the stack
   */
  [[nodiscard]] RAII applyScoped() {
    if (!modified) {
      return RAII{[] {}};
    }
    std::size_t popCount = 0;
    auto index = std::size_t{};
    iterateTuple(
        [&index, &popCount](const auto &value) {
          if (value.has_value()) {
            ImGui::PushStyleColor(static_cast<int>(varArgValueForIndex<SupportedColorTypes...>(index)), *value);
            ++popCount;
          }
          ++index;
        },
        colorValues);

    return RAII{[popCount] { ImGui::PopStyleColor(static_cast<int>(popCount)); }};
  }

 private:
  std::tuple<details::ColorOfAsOptionalColor<SupportedColorTypes>...> colorValues;
  bool modified = false;
};

/**
 * Color palette containing every possible color.
 */
using FullColorPalette = ColorPalette<
    ColorOf::Text, ColorOf::TextDisabled, ColorOf::WindowBackground, ColorOf::ChildBackground, ColorOf::PopupBackground,
    ColorOf::Border, ColorOf::BorderShadow, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
    ColorOf::FrameBackgroundActive, ColorOf::TitleBackground, ColorOf::TitleBackgroundActive,
    ColorOf::TitleBackgroundCollapsed, ColorOf::MenuBarBackground, ColorOf::ScrollbarBackground, ColorOf::ScrollbarGrab,
    ColorOf::ScrollbarGrabHovered, ColorOf::ScrollbarGrabActive, ColorOf::CheckMark, ColorOf::SliderGrab,
    ColorOf::SliderGrabActive, ColorOf::Button, ColorOf::ButtonHovered, ColorOf::ButtonActive, ColorOf::Header,
    ColorOf::HeaderHovered, ColorOf::HeaderActive, ColorOf::Separator, ColorOf::SeparatorHovered,
    ColorOf::SeparatorActive, ColorOf::ResizeGrip, ColorOf::ResizeGripHovered, ColorOf::ResizeGripActive, ColorOf::Tab,
    ColorOf::TabHovered, ColorOf::TabActive, ColorOf::TabUnfocused, ColorOf::TabUnfocusedActive, ColorOf::PlotLines,
    ColorOf::PlotLinesHovered, ColorOf::PlotHistogram, ColorOf::PlotHistogramHovered, ColorOf::TableHeaderBackground,
    ColorOf::TableBorderStrong, ColorOf::TableBorderLight, ColorOf::TableRowBackground, ColorOf::TableRowBackgroundAlt,
    ColorOf::TextSelectedBackground, ColorOf::DragDropTarget, ColorOf::NavHighlight, ColorOf::NavWindowingHighlight,
    ColorOf::NavWindowingDimBackground, ColorOf::ModalWindowDimBackground>;

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_STYLE_COLORPALETTE_H
