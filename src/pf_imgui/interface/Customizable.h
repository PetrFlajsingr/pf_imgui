//
// Created by petr on 6/18/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H

#include <optional>
#include <pf_common/RAII.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/tuple.h>
#include <pf_imgui/styles/enums.h>

namespace pf::ui::ig {

namespace details {
template<style::ColorOf ColorType>
using ColorOfAsImVec4 = ImVec4;

template<style::ColorOf Needle, style::ColorOf HaystackStart, style::ColorOf... Haystack>
consteval std::size_t indexForColorOf() {
  if (Needle == HaystackStart) {
    return 0;
  } else {
    return 1 + indexForColorOf<Needle, Haystack...>();
  }
}
template<style::ColorOf Head, style::ColorOf... Tail>
constexpr style::ColorOf colorOfForIndex(std::size_t index, std::size_t currIndex = 0) {
  if (currIndex == index) { return Head; }
  if constexpr (sizeof...(Tail) == 0) {
    return static_cast<style::ColorOf>(-1);
  } else {
    return colorOfForIndex<Tail...>(index, currIndex + 1);
  }
}

}// namespace details

template<style::ColorOf... SupportedColorTypes>
class ColorCustomizable {
 public:
  [[nodiscard]] RAII setColorStack() {
    auto index = std::size_t{};
    iterateTuple(
        [&index](const auto &value) {
          if (value.has_value()) {
            ImGui::PushStyleColor(static_cast<int>(details::colorOfForIndex<SupportedColorTypes...>(index)), *value);
          }
          ++index;
        },
        colorValues);
    return RAII{[] {
      for (std::size_t i = 0; i < sizeof...(SupportedColorTypes); ++i) { ImGui::PopStyleColor(); }
    }};
  }

  template<style::ColorOf ColorType>
  requires(OneOfValues_v<ColorType, SupportedColorTypes...>) void setColor(ImVec4 color) {
    std::get<details::indexForColorOf<ColorType, SupportedColorTypes...>>(colorValues) = color;
  }

 private:
  std::tuple<std::optional<details::ColorOfAsImVec4<SupportedColorTypes>>...> colorValues;
};

class StyleCustomizable {
 public:
  //[[nodiscard]] RAII setStyleStack();
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMIZABLE_H
