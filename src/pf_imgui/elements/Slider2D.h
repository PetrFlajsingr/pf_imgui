/**
 * @file Slider2D.h
 * @brief Slider2D element.
 * @author Petr Flajšingr
 * @date 18.4.21
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDER2D_H
#define PF_IMGUI_ELEMENTS_SLIDER2D_H

#include <dear_widgets.h>
#include <glm/vec2.hpp>
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <type_traits>

namespace pf::ui::ig {

namespace details {
/**
 * Data storage types for Slider2D.
 */
template<OneOf<int, float> T>
using Slider2DStorageType = std::conditional_t<std::same_as<T, int>, glm::ivec2, glm::vec2>;
}  // namespace details

/**
 * @brief Slider which covers an area and allows a user to select axis value with better visual cues.
 * @tparam T inner type of slider
 */
template<OneOf<int, float> T>
class PF_IMGUI_EXPORT Slider2D : public ItemElement,
                                 public ValueContainer<details::Slider2DStorageType<T>>,
                                 public Savable,
                                 public DragSource<details::Slider2DStorageType<T>>,
                                 public DropTarget<details::Slider2DStorageType<T>> {
 public:
  using StorageType = details::Slider2DStorageType<T>;
  /**
   * @brief Struct for construction of Slider2D.
   */
  struct Config {
    using Parent = Slider2D;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to element */
    Explicit<StorageType> min;        /*!< Minimum allowed value */
    Explicit<StorageType> max;        /*!< Maximum allowed value */
    StorageType value{};              /*!< Initial value */
    Size size = Size::Auto();         /*!< Size of the element */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider2D
   * @param config construction args @see Slider2D::Config
   */
  explicit Slider2D(Config &&config);
  /**
   * Construct Slider2d.
   * @param elementName ID of the slider
   * @param labelText text rendered above the slider
   * @param minMaxX min and max values allowed on X axes
   * @param minMaxY min and max values allowed on Y axes
   * @param initialValue starting value
   * @param initialSize initial size
   * @param persistent enable state saving to disk
   */
  Slider2D(std::string_view elementName, std::string_view labelText, StorageType minMaxX, StorageType minMaxY,
           StorageType initialValue = StorageType{}, Size initialSize = Size::Auto(),
           Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::FrameBackground, ColorOf::Border, ColorOf::BorderShadow,
               ColorOf::FrameBackgroundActive>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Property<Label> label;

  Property<Size> size;
  Property<details::Slider2DStorageType<T>> value;

  [[nodiscard]] const details::Slider2DStorageType<T> &getValue() const override;
  void setValue(const details::Slider2DStorageType<T> &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const details::Slider2DStorageType<T> &)> listener) override;

  void renderImpl() override;

 private:
  StorageType extremesX;
  StorageType extremesY;
};

template<OneOf<int, float> T>
Slider2D<T>::Slider2D(Slider2D::Config &&config)
    : Slider2D(config.name, config.label, {config.min.value.x, config.max.value.x},
               {config.min.value.y, config.max.value.y}, config.value, config.size,
               config.persistent ? Persistent::Yes : Persistent::No) {}

template<OneOf<int, float> T>
Slider2D<T>::Slider2D(std::string_view elementName, std::string_view labelText, Slider2D::StorageType minMaxX,
                      Slider2D::StorageType minMaxY, Slider2D::StorageType initialValue, Size initialSize,
                      Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource<StorageType>(false), DropTarget<StorageType>(false),
      label(std::string{labelText}), size(initialSize), value(initialValue), extremesX(minMaxX), extremesY(minMaxY) {}

template<OneOf<int, float> T>
toml::table Slider2D<T>::toToml() const {
  return toml::table{{"value", serializeGlmVec(*value)}};
}

template<OneOf<int, float> T>
void Slider2D<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
      const auto vecValue = safeDeserializeGlmVec<StorageType>(*newVal);
      if (vecValue.has_value()) { *value.modify() = *vecValue; }
    }
  }
}

template<OneOf<int, float> T>
void Slider2D<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  auto address = &Prop_value(value);
  const auto oldValue = *address;
  if constexpr (std::same_as<T, int>) {
    valueChanged = ImWidgets::Slider2DInt(label->get().c_str(), &address->x, &address->y, &extremesX.x, &extremesX.y,
                                          &extremesY.x, &extremesY.y, static_cast<ImVec2>(*size));
  }
  if constexpr (std::same_as<T, float>) {
    valueChanged = ImWidgets::Slider2DFloat(label->get().c_str(), &address->x, &address->y, extremesX.x, extremesX.y,
                                            extremesY.x, extremesY.y, static_cast<ImVec2>(*size));
  }
  DragSource<StorageType>::drag(*value);
  if (auto drop = DropTarget<StorageType>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged && oldValue != *address) { Prop_triggerListeners(value); }
}

template<OneOf<int, float> T>
const details::Slider2DStorageType<T> &Slider2D<T>::getValue() const {
  return *value;
}

template<OneOf<int, float> T>
Subscription Slider2D<T>::addValueListenerImpl(std::function<void(const details::Slider2DStorageType<T> &)> listener) {
  return value.addListener(std::move(listener));
}

template<OneOf<int, float> T>
void Slider2D<T>::setValue(const details::Slider2DStorageType<T> &newValue) {
  *value.modify() = newValue;
}

extern template class Slider2D<int>;
extern template class Slider2D<float>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDER2D_H
