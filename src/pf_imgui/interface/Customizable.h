/**
 * @file Customizable.h
 * @brief Interface to provide customization functionality to its descendants.
 * @author Petr Flajšingr
 * @date 18.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H

#include <optional>
#include <pf_common/RAII.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/tuple.h>
#include <pf_imgui/styles/enums.h>

namespace pf::ui::ig {

// TODO: style pops can be done by calculating pop count on push
// TODO: call only if changed
namespace details {
template<style::ColorOf ColorType>
using ColorOfAsImVec4 = ImVec4;
template<style::ColorOf ColorType>
using ColorOfAsOptionalImVec4 = std::optional<ColorOfAsImVec4<ColorType>>;
template<style::Style Style>
using TypeForStyle = std::conditional_t<style::isFloatStyle(Style), float, ImVec2>;
template<style::Style Style>
using OptionalTypeForStyle = std::optional<TypeForStyle<Style>>;

// TODO: move this to pf_common
template<auto Needle, auto HaystackStart, auto... Haystack>
consteval std::size_t indexInVarArgList() {
  if (Needle == HaystackStart) {
    return 0;
  } else {
    if constexpr (sizeof...(Haystack) == 0) {
      return -1;
    } else {
      return 1 + indexInVarArgList<Needle, Haystack...>();
    }
  }
}
template<auto Head, auto... Tail>
constexpr auto varArgValueForIndex(std::size_t index, std::size_t currIndex = 0) {
  if (currIndex == index) { return Head; }
  if constexpr (sizeof...(Tail) == 0) {
    return decltype(Head){};
  } else {
    return varArgValueForIndex<Tail...>(index, currIndex + 1);
  }
}

}// namespace details

/**
 * @brief An interface to allow for changing color style. It also provides functionality to apply these styles.
 * @tparam SupportedColorTypes list of types which are supported
 */
template<style::ColorOf... SupportedColorTypes>
class ColorCustomizable {
 public:
#ifdef PF_IMGUI_ENABLE_STYLES
  /**
   * Set color for given type.
   * @tparam ColorType type to set color for
   */
  template<style::ColorOf ColorType>
  requires(OneOfValues_v<ColorType, SupportedColorTypes...>) void setColor(ImVec4 color) {
    std::get<details::indexInVarArgList<ColorType, SupportedColorTypes...>()>(colorValues) = color;
  }
#endif
 protected:
  /**
   * Add color options on imgui stack and return an instance of RAII which resets it.
   * @return RAII resetting the stack
   */
  [[nodiscard]] RAII setColorStack() {
#ifdef PF_IMGUI_ENABLE_STYLES
    std::size_t popCount = 0;
    auto index = std::size_t{};
    iterateTuple(
        [&index, &popCount](const auto &value) {
          if (value.has_value()) {
            ImGui::PushStyleColor(static_cast<int>(details::varArgValueForIndex<SupportedColorTypes...>(index)),
                                  *value);
            ++popCount;
          }
          ++index;
        },
        colorValues);

    return RAII{[popCount] { ImGui::PopStyleColor(popCount); }};
#else
    return RAII{[] {}};
#endif
  }

#ifdef PF_IMGUI_ENABLE_STYLES
 private:
  std::tuple<details::ColorOfAsOptionalImVec4<SupportedColorTypes>...> colorValues;
#endif
};

using AllColorCustomizable = ColorCustomizable<
    style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::WindowBackground,
    style::ColorOf::ChildBackground, style::ColorOf::PopupBackground, style::ColorOf::Border,
    style::ColorOf::BorderShadow, style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
    style::ColorOf::FrameBackgroundActive, style::ColorOf::TitleBackground, style::ColorOf::TitleBackgroundActive,
    style::ColorOf::TitleBackgroundCollapsed, style::ColorOf::MenuBarBackground, style::ColorOf::ScrollbarBackground,
    style::ColorOf::ScrollbarGrab, style::ColorOf::ScrollbarGrabHovered, style::ColorOf::ScrollbarGrabActive,
    style::ColorOf::CheckMark, style::ColorOf::SliderGrab, style::ColorOf::SliderGrabActive, style::ColorOf::Button,
    style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive, style::ColorOf::Header, style::ColorOf::HeaderHovered,
    style::ColorOf::HeaderActive, style::ColorOf::Separator, style::ColorOf::SeparatorHovered,
    style::ColorOf::SeparatorActive, style::ColorOf::ResizeGrip, style::ColorOf::ResizeGripHovered,
    style::ColorOf::ResizeGripActive, style::ColorOf::Tab, style::ColorOf::TabHovered, style::ColorOf::TabActive,
    style::ColorOf::TabUnfocused, style::ColorOf::TabUnfocusedActive, style::ColorOf::PlotLines,
    style::ColorOf::PlotLinesHovered, style::ColorOf::PlotHistogram, style::ColorOf::PlotHistogramHovered,
    style::ColorOf::TableHeaderBackground, style::ColorOf::TableBorderStrong, style::ColorOf::TableBorderLight,
    style::ColorOf::TableRowBackground, style::ColorOf::TableRowBackgroundAlt, style::ColorOf::TextSelectedBackground,
    style::ColorOf::DragDropTarget, style::ColorOf::NavHighlight, style::ColorOf::NavWindowingHighlight,
    style::ColorOf::NavWindowingDimBackground, style::ColorOf::ModalWindowDimBackground>;

/**
 * @brief An interface to allow for changing style. It also provides functionality to apply these styles.
 * @tparam SupportedStyles list of types which are supported
 */
template<style::Style... SupportedStyles>
class StyleCustomizable {
 public:
#ifdef PF_IMGUI_ENABLE_STYLES
  /**
   * Set value for given type.
   * @tparam Style type to set value for
   * @param value value to set
   */
  template<style::Style Style>
  requires(OneOfValues_v<Style, SupportedStyles...> &&style::isFloatStyle(Style)) void setStyle(float value) {
    std::get<details::indexInVarArgList<Style, SupportedStyles...>()>(styleValues) = value;
  }
  /**
   * Set value for given type.
   * @tparam Style type to set value for
   * @param value value to set
   */
  template<style::Style Style>
  requires(OneOfValues_v<Style, SupportedStyles...> && !style::isFloatStyle(Style)) void setStyle(ImVec2 value) {
    std::get<details::indexInVarArgList<Style, SupportedStyles...>()>(styleValues) = value;
  }
#endif
 protected:
  /**
   * Add style options on imgui stack and return an instance of RAII which resets it.
   * @return RAII resetting the stack
   */
  [[nodiscard]] RAII setStyleStack() {
#ifdef PF_IMGUI_ENABLE_STYLES
    std::size_t popCount = 0;
    auto index = std::size_t{};
    iterateTuple(
        [&index, &popCount](const auto &value) {
          if (value.has_value()) {
            ImGui::PushStyleVar(static_cast<int>(details::varArgValueForIndex<SupportedStyles...>(index)), *value);
            ++popCount;
          }
          ++index;
        },
        styleValues);
    return RAII{[popCount] { ImGui::PopStyleVar(popCount); }};
#else
    return RAII{[] {}};
#endif
  };
#ifdef PF_IMGUI_ENABLE_STYLES
 private:
  std::tuple<details::OptionalTypeForStyle<SupportedStyles>...> styleValues;
#endif
};

using AllStyleCustomizable =
    StyleCustomizable<style::Style::Alpha, style::Style::WindowRounding, style::Style::WindowBorderSize,
                      style::Style::ChildRounding, style::Style::ChildBorderSize, style::Style::PopupRounding,
                      style::Style::PopupBorderSize, style::Style::FrameRounding, style::Style::FrameBorderSize,
                      style::Style::IndentSpacing, style::Style::ScrollbarSize, style::Style::ScrollbarRounding,
                      style::Style::GrabMinSize, style::Style::GrabRounding, style::Style::TabRounding,
                      style::Style::WindowPadding, style::Style::WindowMinSize, style::Style::WindowTitleAlign,
                      style::Style::FramePadding, style::Style::ItemSpacing, style::Style::ItemInnerSpacing,
                      style::Style::CellPadding, style::Style::ButtonTextAlign, style::Style::SelectableTextAlign>;
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H
