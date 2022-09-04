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
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Slider with 3D space.
 * @tparam T underlying type
 */
template<OneOf<float> T>
class PF_IMGUI_EXPORT Slider3D : public ItemElement,
                                 public ValueObservable<glm::vec3>,
                                 public Savable,
                                 public DragSource<glm::vec3>,
                                 public DropTarget<glm::vec3> {
 public:
  /**
   * @brief Struct for construction of Slider3D.
   */
  struct Config {
    using Parent = Slider3D;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    Explicit<glm::vec3> min;          /*!< Minimum allowed value */
    Explicit<glm::vec3> max;          /*!< Maximum allowed value */
    glm::vec3 value{};                /*!< Initial value */
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
  Slider3D(const std::string &elementName, const std::string &labelText, const glm::vec2 &minMaxX,
           const glm::vec2 &minMaxY, const glm::vec2 &minMaxZ, const glm::vec3 &initialValue = {},
           Size initialSize = Size::Auto(), Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::FrameBackground, ColorOf::Border, ColorOf::BorderShadow,
               ColorOf::FrameBackgroundActive>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Label label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  glm::vec2 extremesX;
  glm::vec2 extremesY;
  glm::vec2 extremesZ;
};

template<OneOf<float> T>
Slider3D<T>::Slider3D(Slider3D::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable<glm::vec3>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<glm::vec3>(false),
      DropTarget<glm::vec3>(false), size(config.size), label(std::string{config.label.value}),
      extremesX(config.min.value.x, config.max.value.x), extremesY(config.min.value.y, config.max.value.y),
      extremesZ(config.min.value.z, config.max.value.z) {}

template<OneOf<float> T>
Slider3D<T>::Slider3D(const std::string &elementName, const std::string &labelText, const glm::vec2 &minMaxX,
                      const glm::vec2 &minMaxY, const glm::vec2 &minMaxZ, const glm::vec3 &initialValue,
                      Size initialSize, Persistent persistent)
    : ItemElement(elementName), ValueObservable<glm::vec3>(initialValue),
      Savable(persistent), DragSource<glm::vec3>(false), DropTarget<glm::vec3>(false), size(initialSize),
      label(labelText), extremesX(minMaxX), extremesY(minMaxY), extremesZ(minMaxZ) {}

template<OneOf<float> T>
toml::table Slider3D<T>::toToml() const {
  const auto val = ValueObservable<glm::vec3>::getValue();
  return toml::table{{"value", serializeGlmVec(val)}};
}

template<OneOf<float> T>
void Slider3D<T>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
      const auto vecValue = safeDeserializeGlmVec<glm::vec3>(*newVal);
      if (vecValue.has_value()) { ValueObservable<glm::vec3>::setValueAndNotifyIfChanged(*vecValue); }
    }
  }
}

template<OneOf<float> T>
void Slider3D<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  auto address = ValueObservable<glm::vec3>::getValueAddress();
  const auto oldValue = *address;
  if constexpr (std::same_as<T, float>) {
    valueChanged =
        ImWidgets::SliderScalar3D(label.get().c_str(), &address->x, &address->y, &address->z, extremesX.x, extremesX.y,
                                  extremesY.x, extremesY.y, extremesZ.x, extremesZ.y, static_cast<ImVec2>(*size));
  }
  DragSource<glm::vec3>::drag(ValueObservable<glm::vec3>::getValue());
  if (auto drop = DropTarget<glm::vec3>::dropAccept(); drop.has_value()) {
    ValueObservable<glm::vec3>::setValueAndNotifyIfChanged(*drop);
    return;
  }
  if (valueChanged && oldValue != *address) { ValueObservable<glm::vec3>::notifyValueChanged(); }
}

extern template class Slider3D<float>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_SLIDER3D_H
