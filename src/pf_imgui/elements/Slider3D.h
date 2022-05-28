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
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {
/**
 * @brief Slider with 3D space.
 * @tparam T underlying type
 */
template<OneOf<float> T>
class PF_IMGUI_EXPORT Slider3D
    : public ItemElement,
      public Labellable,
      public ValueObservable<glm::vec3>,
      public Savable,
      public DragSource<glm::vec3>,
      public DropTarget<glm::vec3>,
      public Resizable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::FrameBackground, style::ColorOf::Border,
                               style::ColorOf::BorderShadow, style::ColorOf::FrameBackgroundActive>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  /**
   * @brief Struct for construction of Slider3D.
   */
  struct Config {
    using Parent = Slider3D;
    std::string_view name;    /*!< Unique name of the element */
    std::string_view label;   /*!< Text rendered next to the element */
    glm::vec3 min;            /*!< Minimum allowed value */
    glm::vec3 max;            /*!< Maximum allowed value */
    glm::vec3 value{};        /*!< Initial value */
    Size size = Size::Auto(); /*!< Size of the element */
    bool persistent = false;  /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider3D
   * @param config construction args @see Slider3D::Config
   */
  explicit Slider3D(Config &&config);
  /**
   * Construct Slider3D.
   * @param elementName ID of the element
   * @param label text rendered above the slider
   * @param minMaxX extremes on X axis
   * @param minMaxY extremes on Y axis
   * @param minMaxZ extremes on Z axis
   * @param value starting value
   * @param size size of the rendered area
   * @param persistent enable state saving to disk
   */
  Slider3D(const std::string &elementName, const std::string &label, const glm::vec2 &minMaxX, const glm::vec2 &minMaxY,
           const glm::vec2 &minMaxZ, const glm::vec3 &value = {}, Size size = Size::Auto(),
           Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  glm::vec2 extremesX;
  glm::vec2 extremesY;
  glm::vec2 extremesZ;
};

template<OneOf<float> T>
Slider3D<T>::Slider3D(Slider3D::Config &&config)
    : ItemElement(std::string{config.name}),
      Labellable(std::string{config.label}), ValueObservable<glm::vec3>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<glm::vec3>(false),
      DropTarget<glm::vec3>(false), Resizable(config.size), extremesX(config.min.x, config.max.x),
      extremesY(config.min.y, config.max.y), extremesZ(config.min.z, config.max.z) {}

template<OneOf<float> T>
Slider3D<T>::Slider3D(const std::string &elementName, const std::string &label, const glm::vec2 &minMaxX,
                      const glm::vec2 &minMaxY, const glm::vec2 &minMaxZ, const glm::vec3 &value, Size size,
                      Persistent persistent)
    : ItemElement(elementName), Labellable(label), ValueObservable<glm::vec3>(value),
      Savable(persistent), DragSource<glm::vec3>(false), DropTarget<glm::vec3>(false), Resizable(size),
      extremesX(minMaxX), extremesY(minMaxY), extremesZ(minMaxZ) {}

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
      if (vecValue.has_value()) { ValueObservable<glm::vec3>::setValueAndNotifyIfChanged(vecValue.value()); }
    }
  }
}

template<OneOf<float> T>
void Slider3D<T>::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  auto valueChanged = false;
  auto address = ValueObservable<glm::vec3>::getValueAddress();
  const auto oldValue = *address;
  if constexpr (std::same_as<T, float>) {
    valueChanged =
        ImWidgets::SliderScalar3D(getLabel().c_str(), &address->x, &address->y, &address->z, extremesX.x, extremesX.y,
                                  extremesY.x, extremesY.y, extremesZ.x, extremesZ.y, static_cast<ImVec2>(getSize()));
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
