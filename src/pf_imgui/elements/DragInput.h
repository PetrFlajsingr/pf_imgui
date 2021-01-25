//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_DRAGINPUT_H
#define PF_IMGUI_IMGUI_ELEMENTS_DRAGINPUT_H

#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/Savable.h"
#include "interface/ValueObservable.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_common/math/Range.h>
#include <pf_imgui/_export.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
namespace details {
#define IMGUI_DRAG_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4, math::Range<float>
#define IMGUI_DRAG_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4, math::Range<int>
#define IMGUI_DRAG_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_DRAG_RANGE_TYPE_LIST math::Range<int>, math::Range<float>
#define IMGUI_DRAG_TYPE_LIST IMGUI_DRAG_FLOAT_TYPE_LIST, IMGUI_DRAG_INT_TYPE_LIST

template<typename T>
using UnderlyingType = std::conditional_t<OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST>, float, int>;

template<typename T>
constexpr const char *defaultDragFormat() {
  if constexpr (OneOf<T, IMGUI_DRAG_FLOAT_TYPE_LIST> || std::same_as<T, math::Range<float>>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}// namespace details
template<OneOf<IMGUI_DRAG_TYPE_LIST> T>
class PF_IMGUI_EXPORT DragInput : public ItemElement, public ValueObservable<T>, public Labellable, public Savable {
 public:
  using ParamType = details::UnderlyingType<T>;

  DragInput(const std::string &elementName, const std::string &label, ParamType speed, ParamType min, ParamType max,
            Persistent persistent = Persistent::No, T value = T{}, std::string format = details::defaultDragFormat<T>())
      : ItemElement(elementName), ValueObservable<T>(value), Labellable(label), Savable(persistent), speed(speed),
        min(min), max(max), format(std::move(format)) {}

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
    if constexpr (std::same_as<T, float>) {
      valueChanged =
          ImGui::DragFloat(getLabel().c_str(), ValueObservable<T>::getValueAddress(), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::DragFloat2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                       speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::DragFloat3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                       speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::DragFloat4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                       speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged =
          ImGui::DragInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::DragInt2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), speed,
                                     min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::DragInt3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), speed,
                                     min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::DragInt4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), speed,
                                     min, max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<int>>) {
      valueChanged =
          ImGui::DragIntRange2(getLabel().c_str(), reinterpret_cast<int *>(ValueObservable<T>::getValueAddress()),
                               speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<float>>) {
      valueChanged =
          ImGui::DragFloatRange2(getLabel().c_str(), &ValueObservable<T>::getValueAddress()->start,
                                 &ValueObservable<T>::getValueAddress()->end, speed, min, max, format.c_str());
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
#endif//PF_IMGUI_IMGUI_ELEMENTS_DRAGINPUT_H
