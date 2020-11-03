//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DRAGINPUT_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DRAGINPUT_H

#include <pf_common/concepts/OneOf.h>
#include <pf_common/math.h>
#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <utility>
#include <pf_imgui/_export.h>

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
class PF_IMGUI_EXPORT DragInput : public ValueObservableElement<T>, public LabeledElement, public SavableElement {
 public:
  using ParamType = details::UnderlyingType<T>;

  DragInput(const std::string &elementName, const std::string &caption, ParamType speed,
            ParamType min, ParamType max, Persistent persistent = Persistent::No, T value = T{},
            std::string format = details::defaultDragFormat<T>())
      : Element(elementName), ValueObservableElement<T>(elementName, value),
        LabeledElement(elementName, caption), SavableElement(elementName, persistent), speed(speed),
        min(min), max(max), format(std::move(format)) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
      auto range = T{};
      const auto tomlRange = src["value"].as_array();
      range.start = *tomlRange->get(0)->template value<ParamType>();
      range.end = *tomlRange->get(1)->template value<ParamType>();
      ValueObservableElement<T>::setValueAndNotifyIfChanged(range);
    }else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservableElement<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservableElement<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservableElement<T>::getValue();
    if constexpr (OneOf<T, IMGUI_DRAG_RANGE_TYPE_LIST>) {
      return toml::table{{{"value", toml::array{value.start, value.end}}}};
    } else if constexpr (OneOf<T, IMGUI_DRAG_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    const auto oldValue = ValueObservableElement<T>::getValue();
    if constexpr (std::same_as<T, float>) {
      ImGui::DragFloat(getLabel().c_str(), ValueObservableElement<T>::getValueAddress(), speed, min,
                       max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      ImGui::DragFloat2(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                        max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      ImGui::DragFloat3(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                        max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      ImGui::DragFloat4(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                        max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      ImGui::DragInt(getLabel().c_str(), ValueObservableElement<T>::getValueAddress(), speed, min,
                     max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      ImGui::DragInt2(getLabel().c_str(),
                      glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                      max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      ImGui::DragInt3(getLabel().c_str(),
                      glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                      max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      ImGui::DragInt4(getLabel().c_str(),
                      glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), speed, min,
                      max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<int>>) {
      ImGui::DragIntRange2(getLabel().c_str(),
                           reinterpret_cast<int *>(ValueObservableElement<T>::getValueAddress()),
                           speed, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, math::Range<float>>) {
      ImGui::DragFloatRange2(
          getLabel().c_str(), &ValueObservableElement<T>::getValueAddress()->start,
          &ValueObservableElement<T>::getValueAddress()->end, speed, min, max, format.c_str());
    }
    if (oldValue != ValueObservableElement<T>::getValue()) {
      ValueObservableElement<T>::notifyValueChanged();
    }
  }

 private:
  ParamType speed;
  ParamType min;
  ParamType max;
  std::string format;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DRAGINPUT_H
