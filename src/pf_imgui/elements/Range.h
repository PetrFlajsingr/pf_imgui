//
// Created by petr on 4/20/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_RANGE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_RANGE_H

#include <glm/vec2.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

namespace details {
/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultRangeFormat() {
  if constexpr (std::same_as<T, float>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
/**
 * Storage type for Range.
 */
template<typename T>
using RangeStorageType = std::conditional_t<std::same_as<T, float>, glm::vec2, glm::ivec2>;
}// namespace details

/**
 * @brief Range of 2 values.
 * @tparam T type for range
 */
template<OneOf<float, int> T>
class Range : public ItemElement,
              public Labellable,
              public ValueObservable<details::RangeStorageType<T>>,
              public Savable {
  using StorageType = details::RangeStorageType<T>;

 public:
  /**
   * Construct Range.
   * @param elementName ID of the element
   * @param label text rendered next to the element
   * @param persistent enable state saving to disk
   * @param min min range value
   * @param max max range value
   * @param value starting value
   * @param format format for text rendered on the range, printf-like
   */
  Range(const std::string &elementName, const std::string &label, T min, T max, StorageType value = {}, float moveSpeed = 1.0f,
        Persistent persistent = Persistent::No, const std::string &format = details::defaultRangeFormat<T>())
      : ItemElement(elementName), Labellable(label), ValueObservable<StorageType>(value), Savable(persistent),
        minVal(min), maxVal(max), format(format), speed(moveSpeed) {}

 protected:
  void renderImpl() override {
    auto valueChanged = false;
    const auto valueAddress = ValueObservable<StorageType>::getValueAddress();
    if constexpr (std::same_as<T, float>) {
      valueChanged =
          ImGui::DragFloatRange2(getLabel().c_str(), &valueAddress->x, &valueAddress->y, speed, minVal, maxVal, format.c_str());
    } else {
      valueChanged =
          ImGui::DragIntRange2(getLabel().c_str(), &valueAddress->x, &valueAddress->y, speed, minVal, maxVal, format.c_str());
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
  T minVal;
  T maxVal;
  std::string format;
  float speed;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_RANGE_H
