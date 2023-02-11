/**
 * @file StyleOptions.h
 * @brief StyleOptions for element styling.
 * @author Petr Flajšingr
 * @date 17.6.22
 */

#ifndef PF_IMGUI_STYLE_STYLEOPTIONS_H
#define PF_IMGUI_STYLE_STYLEOPTIONS_H

#include <pf_common/ScopeExit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/style/common.h>

namespace pf::ui::ig {

/**
 * @brief A structure storing styling data.
 * @tparam SupportedStyles list of types which are supported
 */
template<StyleOf... SupportedStyles>
class PF_IMGUI_EXPORT StyleOptions {
 public:
  /**
   * Set value for given type.
   * @tparam Style type to set value for
   * @param value value to set
   */
  template<StyleOf Style>
    requires(OneOfValues_v<Style, SupportedStyles...> && details::isFloatStyle(Style))
  void set(float value) {
    modified = true;
    std::get<indexInVarArgList<Style, SupportedStyles...>()>(styleValues) = value;
  }
  /**
   * Set value for given type.
   * @tparam Style type to set value for
   * @param value value to set
   */
  template<StyleOf Style>
    requires(OneOfValues_v<Style, SupportedStyles...> && !details::isFloatStyle(Style))
  void set(ImVec2 value) {
    modified = true;
    std::get<indexInVarArgList<Style, SupportedStyles...>()>(styleValues) = value;
  }
  /**
   * Reset the style to a default value.
   * @tparam Style style to reset
   */
  template<StyleOf Style>
  void unset() {
    std::get<indexInVarArgList<Style, SupportedStyles...>()>(styleValues) = std::nullopt;
  }
  /**
   * Reset all styles to default values.
   */
  void clear() {
    iterateTuple([](auto &value) { value = std::nullopt; }, styleValues);
  }

  /**
   * Add style options on imgui stack and return an instance of ScopeExit which resets it.
   * @return ScopeExit resetting the stack
   */
  [[nodiscard]] ScopeExit<std::function<void()>> applyScoped() {
    if (!modified) {
      return ScopeExit<std::function<void()>>{[] {}};
    }
    std::size_t popCount = 0;
    auto index = std::size_t{};
    iterateTuple(
        [&index, &popCount](const auto &value) {
          if (value.has_value()) {
            ImGui::PushStyleVar(static_cast<int>(varArgValueForIndex<SupportedStyles...>(index)), *value);
            ++popCount;
          }
          ++index;
        },
        styleValues);
    return ScopeExit<std::function<void()>>{[popCount] { ImGui::PopStyleVar(static_cast<int>(popCount)); }};
  }

 private:
  std::tuple<details::OptionalTypeForStyle<SupportedStyles>...> styleValues;
  bool modified = false;
};

using FullStyleOptions =
    StyleOptions<StyleOf::Alpha, StyleOf::WindowRounding, StyleOf::WindowBorderSize, StyleOf::ChildRounding, StyleOf::ChildBorderSize,
                 StyleOf::PopupRounding, StyleOf::PopupBorderSize, StyleOf::FrameRounding, StyleOf::FrameBorderSize, StyleOf::IndentSpacing,
                 StyleOf::ScrollbarSize, StyleOf::ScrollbarRounding, StyleOf::GrabMinSize, StyleOf::GrabRounding, StyleOf::TabRounding,
                 StyleOf::WindowPadding, StyleOf::WindowMinSize, StyleOf::WindowTitleAlign, StyleOf::FramePadding, StyleOf::ItemSpacing,
                 StyleOf::ItemInnerSpacing, StyleOf::CellPadding, StyleOf::ButtonTextAlign, StyleOf::SelectableTextAlign>;

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_STYLE_STYLEOPTIONS_H
