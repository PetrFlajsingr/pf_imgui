//
// Created by petr on 4/23/21.
//

#ifndef PF_IMGUI_ELEMENTS_VERTICALSLIDER_H
#define PF_IMGUI_ELEMENTS_VERTICALSLIDER_H

#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
namespace details {
/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultVSliderFormat() {
  if constexpr (std::same_as<T, float>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}  // namespace details

/**
 * @brief Slider rendered vertically.
 * @tparam T inner value type
 */
template<OneOf<float, int> T>
class PF_IMGUI_EXPORT VerticalSlider : public ItemElement,
                                       public ValueContainer<T>,
                                       public Savable,
                                       public DragSource<T>,
                                       public DropTarget<T> {
 public:
  /**
   * @brief Struct for construction of VerticalSlider.
   */
  struct Config {
    using Parent = VerticalSlider;
    Explicit<std::string_view> name;                         /*!< Unique name of the element */
    Explicit<std::string_view> label;                        /*!< Text rendered next to the element */
    Explicit<T> min;                                         /*!< Minimum allowed value */
    Explicit<T> max;                                         /*!< Maximum allowed value */
    T value{};                                               /*!< Initial value */
    Explicit<Size> size;                                     /*!< Size of the element */
    std::string format = details::defaultVSliderFormat<T>(); /*!< Format string used to render value */
    bool persistent = false;                                 /*!< Allow state saving to disk */
  };
  /**
   * Construct VerticalSlider
   * @param config construction args @see VerticalSlider::Config
   */
  explicit VerticalSlider(Config &&config);
  /**
   * Construct VerticalSlider.
   * @param elementName ID of the element
   * @param labelText text rendered next to the slider
   * @param initialSize initial size of the element
   * @param minVal min allowed value
   * @param maxVal max allowed value
   * @param initialValue starting value
   * @param persistent enable state saving to disk
   * @param numberFormat printf-like format for value
   */
  VerticalSlider(std::string_view elementName, std::string_view labelText, Size initialSize, T minVal, T maxVal,
                 T initialValue = T{}, Persistent persistent = Persistent::No,
                 std::string numberFormat = details::defaultVSliderFormat<T>());

  /**
   * Get min slider value.
   * @return min slider value
   */
  [[nodiscard]] T getMin() const { return min; }
  /**
   * Set min slider value.
   * @param min new min slider value
   */
  void setMin(const T &newMin) { min = newMin; }
  /**
   * Get max slider value.
   * @return max slider value
   */
  [[nodiscard]] T getMax() const { return max; }
  /**
   * Set max slider value.
   * @param max new min slider value
   */
  void setMax(const T &newMax) { max = newMax; }

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

  Observable<Size> size;
  ObservableProperty<VerticalSlider, T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

 private:
  T min;
  T max;
  std::string format;
};

template<OneOf<float, int> T>
VerticalSlider<T>::VerticalSlider(VerticalSlider::Config &&config)
    : ItemElement(config.name.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{config.label.value}), size(config.size.value), value(config.value), min(config.min),
      max(config.max), format(std::move(config.format)) {}

template<OneOf<float, int> T>
VerticalSlider<T>::VerticalSlider(std::string_view elementName, std::string_view labelText, Size initialSize, T minVal,
                                  T maxVal, T initialValue, Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{labelText}), size(initialSize), value(initialValue), min(minVal), max(maxVal),
      format(std::move(numberFormat)) {}

template<OneOf<float, int> T>
toml::table VerticalSlider<T>::toToml() const {
  return toml::table{{"value", *value}};
}

template<OneOf<float, int> T>
void VerticalSlider<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) { *value.modify() = *newVal; }
  }
}

template<OneOf<float, int> T>
void VerticalSlider<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto address = &value.value;
  const auto flags = ImGuiSliderFlags_AlwaysClamp;

  ImGuiDataType_ dataType;
  if constexpr (std::same_as<T, float>) {
    dataType = ImGuiDataType_Float;
  } else {
    dataType = ImGuiDataType_S32;
  }
  const auto valueChanged = ImGui::VSliderScalar(label->get().c_str(), static_cast<ImVec2>(*size), dataType, address,
                                                 &min, &max, format.c_str(), flags);

  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged) { value.triggerListeners(); }
}

template<OneOf<float, int> T>
const T &VerticalSlider<T>::getValue() const {
  return *value;
}

template<OneOf<float, int> T>
void VerticalSlider<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

template<OneOf<float, int> T>
Subscription VerticalSlider<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

extern template class VerticalSlider<int>;
extern template class VerticalSlider<float>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_VERTICALSLIDER_H
