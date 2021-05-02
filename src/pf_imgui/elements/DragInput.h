/**
 * @file DragInput.h
 * @brief DragInput element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_DRAGINPUT_H
#define PF_IMGUI_ELEMENTS_DRAGINPUT_H

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/math/Range.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
namespace details {
/**
 * Types with float as underlying value.
 */
#define IMGUI_DRAG_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4, math::Range<float>
/**
 * Types with int as underlying value.
 */
#define IMGUI_DRAG_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>
/**
 * Used glm types.
 */
#define IMGUI_DRAG_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
 * Used range types.
 */
#define IMGUI_DRAG_RANGE_TYPE_LIST math::Range<int>, math::Range<float>
/**
 * All allowed types for DragInput
 */
#define IMGUI_DRAG_TYPE_LIST IMGUI_DRAG_FLOAT_TYPE_LIST, IMGUI_DRAG_INT_TYPE_LIST
/**
 * Underlying type of supported types.
 */
template<typename T>
using DragInputUnderlyingType = std::conditional_t<OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST>, float, int>;
/**
 * Default formatting string for supported types.
 * @tparam T type to based format on
 * @return printf like format for numbers
 */
template<typename T>
constexpr const char *defaultDragFormat() {
  if constexpr (OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST> || std::same_as<T, math::Range<float>>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}// namespace details

/**
 * @brief Numeric input supporting either keyboard input or mouse drag.
 *
 * Type of the DragInput is based on underlying types. There is a separate input for each scalar part of type T.
 *
 * Supported types: int, float, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>, math::Range<float>
 *
 * @tparam T Underlying type
 */
template<OneOf<IMGUI_DRAG_TYPE_LIST> T>
class PF_IMGUI_EXPORT DragInput : public ItemElement,
                                  public ValueObservable<T>,
                                  public Labellable,
                                  public Savable,
                                  public DragSource<T>,
                                  public DropTarget<T> {
 public:
  using ParamType = details::DragInputUnderlyingType<T>;

  /**
   * Construct DragInput.
   * @param elementName ID of the DragInput
   * @param label text drawn next to the input
   * @param speed frequency of value change based on mouse movement distance
   * @param min minimum allowed value
   * @param max maximum allowed value
   * @param persistent allow state saving to disk
   * @param value starting value
   * @param format format for formatting value to string
   */
  DragInput(const std::string &elementName, const std::string &label, ParamType speed, ParamType min, ParamType max,
            T value = T{}, Persistent persistent = Persistent::No, std::string format = details::defaultDragFormat<T>())
      : ItemElement(elementName), ValueObservable<T>(value), Labellable(label),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), speed(speed), min(min), max(max),
        format(std::move(format)) {}

  /**
   * Get movement speed.
   * @return movement speed
   */
  [[nodiscard]] ParamType getSpeed() const { return speed; }
  /**
   * Set movement speed.
   * @param speed new speed
   */
  void setSpeed(ParamType speed) { DragInput::speed = speed; }
  /**
   * Get min drag value.
   * @return min drag value
   */
  [[nodiscard]] ParamType getMin() const { return min; }
  /**
   * Set min drag value.
   * @param min new min drag value
   */
  void setMin(ParamType min) { DragInput::min = min; }
  /**
   * Get max drag value.
   * @return max drag value
   */
  [[nodiscard]] ParamType getMax() const { return max; }
  /**
   * Set max drag value.
   * @param max new min drag value
   */
  void setMax(ParamType max) { DragInput::max = max; }

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
      auto range = T{};
      const auto tomlRange = src["value"].as_array();
      range.start = *tomlRange->get(0)->template value<ParamType>();
      range.end = *tomlRange->get(1)->template value<ParamType>();
      ValueObservable<T>::setValueAndNotifyIfChanged(range);
    } else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservable<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservable<T>::getValue();
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
      return toml::table{{{"value", toml::array{value.start, value.end}}}};
    } else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    bool valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::DragFloat(getLabel().c_str(), address, speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::DragFloat2(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::DragFloat3(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::DragFloat4(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::DragInt(getLabel().c_str(), address, speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::DragInt2(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::DragInt3(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::DragInt4(getLabel().c_str(), glm::value_ptr(*address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<int>>) {
      valueChanged =
          ImGui::DragIntRange2(getLabel().c_str(), reinterpret_cast<int *>(address), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<float>>) {
      valueChanged =
          ImGui::DragFloatRange2(getLabel().c_str(), &address->start, &address->end, speed, min, max, format.c_str());
    }
    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  ParamType speed;
  ParamType min;
  ParamType max;
  std::string format;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_DRAGINPUT_H
