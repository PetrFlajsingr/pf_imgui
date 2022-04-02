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
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
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
class PF_IMGUI_EXPORT Slider2D
    : public ItemElement,
      public Labellable,
      public ValueObservable<details::Slider2DStorageType<T>>,
      public Savable,
      public DragSource<details::Slider2DStorageType<T>>,
      public DropTarget<details::Slider2DStorageType<T>>,
      public Resizable,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::FrameBackground, style::ColorOf::Border,
                               style::ColorOf::BorderShadow, style::ColorOf::FrameBackgroundActive>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  using StorageType = details::Slider2DStorageType<T>;
  /**
   * @brief Struct for construction of Slider2D.
   */
  struct Config {
    using Parent = Slider2D;
    std::string_view name;                  /*!< Unique name of the element */
    std::string_view label;                 /*!< Text rendered next to element */
    StorageType min;                        /*!< Minimum allowed value */
    StorageType max;                        /*!< Maximum allowed value */
    StorageType value{};                    /*!< Initial value */
    Size size = Size::Auto();               /*!< Size of the element */
    Persistent persistent = Persistent::No; /*!< Allow state saving to disk */
  };
  /**
   * Construct Slider2D
   * @param config construction args @see Slider2D::Config
   */
  explicit Slider2D(Config &&config)
      : ItemElement(std::string{config.name}),
        Labellable(std::string{config.label}), ValueObservable<StorageType>(config.value),
        Savable(config.persistent), DragSource<StorageType>(false), DropTarget<StorageType>(false),
        Resizable(config.size), extremesX(config.min.x, config.max.x), extremesY(config.min.y, config.max.y) {}
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
           StorageType value = StorageType{}, Size size = Size::Auto(), Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable<StorageType>(value),
        Savable(persistent), DragSource<StorageType>(false), DropTarget<StorageType>(false), Resizable(size),
        extremesX(minMaxX), extremesY(minMaxY) {}

 protected:
  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    auto valueChanged = false;
    auto address = ValueObservable<StorageType>::getValueAddress();
    const auto oldValue = *address;
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImWidgets::Slider2DInt(getLabel().c_str(), &address->x, &address->y, &extremesX.x, &extremesX.y,
                                            &extremesY.x, &extremesY.y, static_cast<ImVec2>(getSize()));
    }
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImWidgets::Slider2DFloat(getLabel().c_str(), &address->x, &address->y, extremesX.x, extremesX.y,
                                              extremesY.x, extremesY.y, static_cast<ImVec2>(getSize()));
    }
    DragSource<StorageType>::drag(ValueObservable<StorageType>::getValue());
    if (auto drop = DropTarget<StorageType>::dropAccept(); drop.has_value()) {
      ValueObservable<StorageType>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged && oldValue != *address) { ValueObservable<StorageType>::notifyValueChanged(); }
  }

  void unserialize_impl(const toml::table &src) override {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<StorageType>(*newVal);
        if (vecValue.has_value()) { ValueObservable<StorageType>::setValueAndNotifyIfChanged(vecValue.value()); }
      }
    }
  }

  [[nodiscard]] toml::table serialize_impl() const override {
    const auto value = ValueObservable<StorageType>::getValue();
    return toml::table{{"value", serializeGlmVec(value)}};
  }

 private:
  StorageType extremesX;
  StorageType extremesY;
};
#ifdef PF_IMGUI_ENABLE_EXTERN_TEMPLATE
extern template class Slider2D<int>;
extern template class Slider2D<float>;
#endif
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_SLIDER2D_H
