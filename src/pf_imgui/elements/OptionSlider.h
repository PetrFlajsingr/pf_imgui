/**
 * @file OptionSlider.h
 * @brief OptionSlider element.
 * @author Petr Flajšingr
 * @date 15.6.22
 */

#ifndef PF_IMGUI_ELEMENTS_OPTION_SLIDER_H
#define PF_IMGUI_ELEMENTS_OPTION_SLIDER_H

#include <pf_common/concepts/StringConvertible.h>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>

namespace pf::ui::ig {

/**
 * @brief A slider class with predefined options.
 * @tparam T type of stored selectable value
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT OptionSlider : public ItemElement,
                                     public ValueContainer<T>,
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
  OptionSlider(std::string_view elementName, std::string_view label, RangeOf<T> auto &&newValues,
               Persistent persistent = Persistent::No);

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
  Observable<T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

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
OptionSlider<T>::OptionSlider(std::string_view elementName, std::string_view label, RangeOf<T> auto &&newValues,
                              Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{label}),
      value(*std::ranges::begin(newValues)), values{std::ranges::begin(newValues), std::ranges::end(newValues)},
      selectedValueStr(toString(values[0])) {
  value.addListener([this](const auto &newValue) {
    if (const auto iter =
            std::ranges::find_if(values, [&](const auto &val) { return toString(val) == toString(newValue); });
        iter != values.end()) {
      selectedValueIndex = static_cast<int>(iter - values.begin());
      selectedValueStr = toString(values[selectedValueIndex]);
    }
  });
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
        *value.modify() = values[selectedValueIndex];
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
    *value.modify() = values[selectedValueIndex];
  }

  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) { *value.modify() = *drop; }
}

template<ToStringConvertible T>
const T &OptionSlider<T>::getValue() const {
  return *value;
}

template<ToStringConvertible T>
Subscription OptionSlider<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

template<ToStringConvertible T>
void OptionSlider<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_OPTION_SLIDER_H
