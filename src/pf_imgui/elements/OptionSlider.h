/**
 * @file OptionSlider.h
 * @brief OptionSlider element.
 * @author Petr Flajšingr
 * @date 15.6.22
 */

#ifndef PF_IMGUI_ELEMENTS_OPTION_SLIDER
#define PF_IMGUI_ELEMENTS_OPTION_SLIDER

#include <pf_common/concepts/StringConvertible.h>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

/**
 * @brief A slider class with predefined options.
 * @tparam T type of stored selectable value
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT OptionSlider : public ItemElement,
                                     public ValueObservable<T>,
                                     public Savable,
                                     public DragSource<T>,
                                     public DropTarget<T> {
 public:
  /**
   * @brief Construction args for OptionSlider /.
   */
  struct Config {
    using Parent = OptionSlider<T>;
    Explicit<std::string> name;      /*!< Unique name of the element */
    Explicit<std::string> label;     /*!< Text rendered next to the element */
    Explicit<std::vector<T>> values; /*!< Possible options */
    bool persistent = false;         /*!< Enable disk state saving */
  };
  /**
   * Construct OptionSlider.
   * @param config construction args @see OptionSlider::Config
   */
  explicit OptionSlider(Config &&config);
  /**
   * Construct OptionSlider.
   * @param elementName unique name of the element
   * @param label text rendered next to the element
   * @param newValues possible options
   * @param persistent enable disk state saving
   */
  OptionSlider(const std::string &elementName, const std::string &label, RangeOf<T> auto &&newValues,
               Persistent persistent = Persistent::No);

  void setValue(const T &newValue) override;

  [[nodiscard]] toml::table toToml() const override;

  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget,
               ColorOf::SliderGrab, ColorOf::SliderGrabActive, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;

 protected:
  void renderImpl() override;

 private:
  std::vector<T> values;
  int selectedValueIndex{};
  std::string selectedValueStr;
};

template<ToStringConvertible T>
OptionSlider<T>::OptionSlider(OptionSlider::Config &&config)
    : OptionSlider(config.name, config.label, config.values, config.persistent ? Persistent::Yes : Persistent::No) {}

template<ToStringConvertible T>
OptionSlider<T>::OptionSlider(const std::string &elementName, const std::string &label, RangeOf<T> auto &&newValues,
                              Persistent persistent)
    : ItemElement(elementName), ValueObservable<T>(*std::ranges::begin(newValues)),
      Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(label), values{std::ranges::begin(newValues), std::ranges::end(newValues)},
      selectedValueStr(toString(values[0])) {}

template<ToStringConvertible T>
void OptionSlider<T>::setValue(const T &newValue) {
  if (const auto iter =
          std::ranges::find_if(values, [&](const auto &val) { return toString(val) == toString(newValue); });
      iter != values.end()) {
    selectedValueIndex = static_cast<int>(iter - values.begin());
    selectedValueStr = toString(values[selectedValueIndex]);
    ValueObservable<T>::setValue(values[selectedValueIndex]);
  }
}
template<ToStringConvertible T>
toml::table OptionSlider<T>::toToml() const {
  return toml::table{{"selected", selectedValueStr}};
}
template<ToStringConvertible T>
void OptionSlider<T>::setFromToml(const toml::table &src) {
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

template<ToStringConvertible T>
void OptionSlider<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  if (ImGui::SliderInt(label->get().c_str(), &selectedValueIndex, 0, static_cast<int>(values.size() - 1),
                       selectedValueStr.c_str(), flags)) {
    selectedValueStr = toString(values[selectedValueIndex]);
    ValueObservable<T>::setValueAndNotifyIfChanged(values[selectedValueIndex]);
  }

  DragSource<T>::drag(ValueObservable<T>::getValue());
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
  }
}

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_OPTION_SLIDER
