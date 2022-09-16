/**
 * @file Slider3D.h
 * @brief Slider3D element.
 * @author Petr Flajšingr
 * @date 20.4.21
 */

#ifndef PF_IMGUI_ELEMENTS_SLIDER3D_H
#define PF_IMGUI_ELEMENTS_SLIDER3D_H

#include <dear_widgets.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Slider with 3D space.
 * @tparam T underlying type
 */
template<OneOf<glm::vec3> T>
class PF_IMGUI_EXPORT Slider3D : public ItemElement,
                                 public ValueContainer<T>,
                                 public Savable,
                                 public DragSource<T>,
                                 public DropTarget<T> {
 public:
  /**
   * @brief Struct for construction of Slider3D.
   */
  struct Config {
    using Parent = Slider3D;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    Explicit<T> min;                  /*!< Minimum allowed value */
    Explicit<T> max;                  /*!< Maximum allowed value */
    T value{};                        /*!< Initial value */
    Size size = Size::Auto();         /*!< Size of the element */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider3D
   * @param config construction args @see Slider3D::Config
   */
  explicit Slider3D(Config &&config);
  /**
   * Construct Slider3D.
   * @param elementName ID of the element
   * @param labelText text rendered above the slider
   * @param minMaxX extremes on X axis
   * @param minMaxY extremes on Y axis
   * @param minMaxZ extremes on Z axis
   * @param initialValue starting value
   * @param initialSize size of the rendered area
   * @param persistent enable state saving to disk
   */
  Slider3D(std::string_view elementName, std::string_view labelText, glm::vec2 minMaxX, glm::vec2 minMaxY,
           glm::vec2 minMaxZ, const T &initialValue = {}, Size initialSize = Size::Auto(),
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
  Property<T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

 private:
  glm::vec2 extremesX;
  glm::vec2 extremesY;
  glm::vec2 extremesZ;
};

template<OneOf<glm::vec3> T>
Slider3D<T>::Slider3D(Slider3D::Config &&config)
    : Slider3D(config.name, config.label, {config.min.value.x, config.max.value.x},
               {config.min.value.y, config.max.value.y}, {config.min.value.z, config.max.value.z}, config.value,
               config.size, config.persistent ? Persistent::Yes : Persistent::No) {}

template<OneOf<glm::vec3> T>
Slider3D<T>::Slider3D(std::string_view elementName, std::string_view labelText, glm::vec2 minMaxX, glm::vec2 minMaxY,
                      glm::vec2 minMaxZ, const T &initialValue, Size initialSize, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{labelText}), size(initialSize), value(initialValue), extremesX(minMaxX), extremesY(minMaxY),
      extremesZ(minMaxZ) {}

template<OneOf<glm::vec3> T>
toml::table Slider3D<T>::toToml() const {
  return toml::table{{"value", serializeGlmVec(*value)}};
}

template<OneOf<glm::vec3> T>
void Slider3D<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
      const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
      if (vecValue.has_value()) { *value.modify() = *vecValue; }
    }
  }
}

template<OneOf<glm::vec3> T>
void Slider3D<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  auto address = &Prop_value(value);
  const auto oldValue = *address;
  if constexpr (std::same_as<T, float>) {
    valueChanged =
        ImWidgets::SliderScalar3D(label->get().c_str(), &address->x, &address->y, &address->z, extremesX.x, extremesX.y,
                                  extremesY.x, extremesY.y, extremesZ.x, extremesZ.y, static_cast<ImVec2>(*size));
  }
  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged && oldValue != *address) { Prop_triggerListeners(value); }
}

template<OneOf<glm::vec3> T>
const T &Slider3D<T>::getValue() const {
  return *value;
}

template<OneOf<glm::vec3> T>
void Slider3D<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

template<OneOf<glm::vec3> T>
Subscription Slider3D<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

extern template class Slider3D<glm::vec3>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDER3D_H
