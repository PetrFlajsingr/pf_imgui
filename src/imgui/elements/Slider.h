//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_SLIDER_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_SLIDER_H

#include <pf_common/concepts/OneOf.h>
#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <utility>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
//TODO: angle

namespace details {
#define IMGUI_SLIDER_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
#define IMGUI_SLIDER_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_SLIDER_GLM_TYPE_LIST                                                                 \
  glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_SLIDER_TYPE_LIST IMGUI_SLIDER_FLOAT_TYPE_LIST, IMGUI_SLIDER_INT_TYPE_LIST
template<OneOf<IMGUI_SLIDER_TYPE_LIST> T>
using SliderMinMaxType = std::conditional_t<OneOf<T, IMGUI_SLIDER_FLOAT_TYPE_LIST>, float, int>;

template<typename T>
constexpr const char *defaultSliderFormat() {
  if constexpr (OneOf<T, IMGUI_SLIDER_FLOAT_TYPE_LIST>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}// namespace details
template<OneOf<IMGUI_SLIDER_TYPE_LIST> T>
class PF_IMGUI_EXPORT Slider : public LabeledElement, public ValueObservableElement<T>, public SavableElement {
 public:
  using MinMaxType = details::SliderMinMaxType<T>;
  Slider(const std::string &elementName, const std::string &caption, MinMaxType min, MinMaxType max,
         T value = T{}, Persistent persistent = Persistent::No,
         std::string format = details::defaultSliderFormat<MinMaxType>())
      : Element(elementName),
        LabeledElement(elementName, caption), ValueObservableElement<T>(elementName, value),
        SavableElement(elementName, persistent), min(min), max(max), format(std::move(format)) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservableElement<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservableElement<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservableElement<T>::getValue();
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    const auto oldValue = ValueObservableElement<T>::getValue();
    if constexpr (std::same_as<T, float>) {
      ImGui::SliderFloat(getLabel().c_str(), ValueObservableElement<T>::getValueAddress(), min, max,
                         format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      ImGui::SliderFloat2(getLabel().c_str(),
                          glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                          format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      ImGui::SliderFloat3(getLabel().c_str(),
                          glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                          format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      ImGui::SliderFloat4(getLabel().c_str(),
                          glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                          format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      ImGui::SliderInt(getLabel().c_str(), ValueObservableElement<T>::getValueAddress(), min, max,
                       format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      ImGui::SliderInt2(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                        format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      ImGui::SliderInt3(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                        format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      ImGui::SliderInt4(getLabel().c_str(),
                        glm::value_ptr(*ValueObservableElement<T>::getValueAddress()), min, max,
                        format.c_str());
    }
    if (oldValue != ValueObservableElement<T>::getValue()) {
      ValueObservableElement<T>::notifyValueChanged();
    }
  }

 private:
  MinMaxType min;
  MinMaxType max;
  std::string format;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_SLIDER_H
