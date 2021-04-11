//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_SLIDER_H
#define PF_IMGUI_IMGUI_ELEMENTS_SLIDER_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

namespace details {
#define IMGUI_SLIDER_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
#define IMGUI_SLIDER_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_SLIDER_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
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
class PF_IMGUI_EXPORT Slider : public ItemElement, public Labellable, public ValueObservable<T>, public Savable {
 public:
  using MinMaxType = details::SliderMinMaxType<T>;
  Slider(const std::string &elementName, const std::string &label, MinMaxType min, MinMaxType max, T value = T{},
         Persistent persistent = Persistent::No, std::string format = details::defaultSliderFormat<MinMaxType>())
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value), Savable(persistent), min(min), max(max),
        format(std::move(format)) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservable<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservable<T>::getValue();
    if constexpr (OneOf<T, IMGUI_SLIDER_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    auto valueChanged = false;
    if constexpr (std::same_as<T, float>) {
      valueChanged =
          ImGui::SliderFloat(getLabel().c_str(), ValueObservable<T>::getValueAddress(), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::SliderFloat2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                         min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::SliderFloat3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                         min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::SliderFloat4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                         min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged =
          ImGui::SliderInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), min, max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::SliderInt2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), min,
                                       max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::SliderInt3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), min,
                                       max, format.c_str());
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::SliderInt4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()), min,
                                       max, format.c_str());
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  MinMaxType min;
  MinMaxType max;
  std::string format;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_SLIDER_H
