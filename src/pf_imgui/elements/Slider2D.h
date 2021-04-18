/**
 * @file Slider2D.h
 * @brief Slider2D element.
 * @author Petr Flajšingr
 * @date 18.4.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SLIDER2D_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SLIDER2D_H

#include <glm/vec2.hpp>
#include <include/DearWidgets/dear_widgets.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <type_traits>

namespace pf::ui::ig {

namespace details {
template<OneOf<int, float> T>
using Slider2DStorageType = std::conditional_t<std::same_as<T, int>, glm::ivec2, glm::vec2>;
}

/**
 * @brief Slider which covers an area and allows a user to select axis value with better visual cues.
 * @tparam T inner type of slider
 */
template<OneOf<int, float> T>
class Slider2D : public ItemElement,
                 public Labellable,
                 public ValueObservable<details::Slider2DStorageType<T>>,
                 public Savable {
 public:
  using StorageType = details::Slider2DStorageType<T>;
  /**
   * Construct Slider2d.
   * @param elementName ID of the slider
   * @param label text rendered above the slider
   * @param minMaxX min and max values allowed on X axes
   * @param minMaxY min and max values allowed on Y axes
   * @param value starting value
   * @param persistent enable state saving to disk
   */
  Slider2D(const std::string &elementName, const std::string &label, StorageType minMaxX, StorageType minMaxY,
           StorageType value = StorageType{}, Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable<StorageType>(value), Savable(persistent),
        extremesX(minMaxX), extremesY(minMaxY) {}

 protected:
  void renderImpl() override {
    auto valueChanged = false;
    auto valAddress = ValueObservable<StorageType>::getValueAddress();
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImWidgets::Slider2DInt(getLabel().c_str(), &valAddress->x, &valAddress->y, &extremesX.x,
                                            &extremesX.y, &extremesY.x, &extremesY.y);
    }
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImWidgets::Slider2DFloat(getLabel().c_str(), &valAddress->x, &valAddress->y, extremesX.x,
                                              extremesX.y, extremesY.x, extremesY.y);
    }
    if (valueChanged) { ValueObservable<StorageType>::notifyValueChanged(); }
  }

  void unserialize_impl(const toml::table &src) override {
    const auto tomlVec = src["value"].as_array();
    const auto vec = deserializeGlmVec<StorageType>(*tomlVec);
    ValueObservable<StorageType>::setValueAndNotifyIfChanged(vec);
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservable<StorageType>::getValue();
    return toml::table{{{"value", serializeGlmVec(value)}}};
  }

 private:
  StorageType extremesX;
  StorageType extremesY;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SLIDER2D_H
