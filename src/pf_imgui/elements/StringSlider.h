//
// Created by Petr on 6/15/2022.
//

#ifndef PF_IMGUI_ELEMENTS_STRING_SLIDER
#define PF_IMGUI_ELEMENTS_STRING_SLIDER

#include <pf_common/concepts/StringConvertible.h>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

template<ToStringConvertible T>
class PF_IMGUI_EXPORT StringSlider
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget,
                               style::ColorOf::SliderGrab, style::ColorOf::SliderGrabActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  struct Config {
    using Parent = StringSlider<T>;
    Explicit<std::string> name;
    Explicit<std::string> label;
    Explicit<std::vector<T>> values;
    bool persistent = false;
  };
  StringSlider(const std::string &elementName, const std::string &label, RangeOf<T> auto &&newValues,
               Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(*std::ranges::begin(newValues)),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), values{std::ranges::begin(newValues),
                                                                                std::ranges::end(newValues)},
        selectedValueStr(toString(values[0])) {}

  [[nodiscard]] toml::table toToml() const override { return toml::table{{"selected", selectedValueStr}}; }
  void setFromToml(const toml::table &src) override {
    if (auto selectedValIter = src.find("selected"); selectedValIter != src.end()) {
      if (auto selectedVal = selectedValIter->second.as_string(); selectedVal != nullptr) {
        if (const auto iter =
                std::ranges::find_if(values, [&](const auto &val) { return toString(val) == *selectedVal; });
            iter != values.end()) {
          selectedValueIndex = static_cast<int>(iter - values.begin());
          selectedValueStr = toString(values[selectedValueIndex]);
        }
      }
    }
  }

 protected:
  void renderImpl() override {
    [[maybe_unused]] auto colorStyle = setColorStack();
    [[maybe_unused]] auto style = setStyleStack();
    const auto flags = ImGuiSliderFlags_AlwaysClamp;

    if (ImGui::SliderInt(getLabel().c_str(), &selectedValueIndex, 0, static_cast<int>(values.size() - 1),
                         selectedValueStr.c_str(), flags)) {
      selectedValueStr = toString(values[selectedValueIndex]);
      ValueObservable<T>::setValueAndNotifyIfChanged(values[selectedValueIndex]);
    }

    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
    }
  }

 private:
  std::vector<T> values;
  int selectedValueIndex{};
  std::string selectedValueStr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_STRING_SLIDER
