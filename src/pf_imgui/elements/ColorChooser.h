//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H
#define PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H

#include "../serialization.h"
#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/Savable.h"
#include "interface/ValueObservable.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <string>
#include <toml++/toml_node_view.h>

namespace pf::ui::ig {

template<ColorChooserType Type, OneOf<glm::vec3, glm::vec4> T>
class PF_IMGUI_EXPORT ColorChooser : public ItemElement, public Labellable, public ValueObservable<T>, public Savable {
 public:
  ColorChooser(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
               T value = T{})
      : Labellable(label), ValueObservable<T>(value), Savable(persistent) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    const auto tomlColor = src["color"].as_array();
    const auto color = deserializeGlmVec<T>(*tomlColor);
    ValueObservable<T>::setValueAndNotifyIfChanged(color);
  }

  toml::table serialize_impl() override {
    const auto color = ValueObservable<T>::getValue();
    const auto tomlColor = serializeGlmVec(color);
    return toml::table{{{"color", tomlColor}}};
  }

  void renderImpl() override {
    auto valueChanged = false;
    if constexpr (Type == ColorChooserType::Edit) {
      if constexpr (std::same_as<glm::vec3, T>) {
        valueChanged = ImGui::ColorEdit3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
      } else {
        valueChanged = ImGui::ColorEdit4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
      }
    } else {
      if constexpr (std::same_as<glm::vec3, T>) {
        valueChanged = ImGui::ColorPicker3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
      } else {
        valueChanged = ImGui::ColorPicker4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
      }
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H
