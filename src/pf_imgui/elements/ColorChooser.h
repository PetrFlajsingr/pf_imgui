//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H
#define PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H

#include "../serialization.h"
#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/Savable.h"
#include "interface/ValueObservableElement.h"
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
class PF_IMGUI_EXPORT ColorChooser : public ItemElement,
                                     public Labellable,
                                     public ValueObservableElement<T>,
                                     public Savable {
 public:
  ColorChooser(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
               T value = T{})
      : Element(elementName), Labellable(label), ValueObservableElement<T>(elementName, value), Savable(persistent) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    const auto tomlColor = src["color"].as_array();
    const auto color = deserializeGlmVec<T>(*tomlColor);
    ValueObservableElement<T>::setValueAndNotifyIfChanged(color);
  }

  toml::table serialize_impl() override {
    const auto color = ValueObservableElement<T>::getValue();
    const auto tomlColor = serializeGlmVec(color);
    return toml::table{{{"color", tomlColor}}};
  }

  void renderImpl() override {
    const auto oldValue = ValueObservableElement<T>::getValue();
    if constexpr (Type == ColorChooserType::Edit) {
      if constexpr (std::same_as<glm::vec3, T>) {
        ImGui::ColorEdit3(getLabel().c_str(), glm::value_ptr(*ValueObservableElement<T>::getValueAddress()));
      } else {
        ImGui::ColorEdit4(getLabel().c_str(), glm::value_ptr(*ValueObservableElement<T>::getValueAddress()));
      }
    } else {
      if constexpr (std::same_as<glm::vec3, T>) {
        ImGui::ColorPicker3(getLabel().c_str(), glm::value_ptr(*ValueObservableElement<T>::getValueAddress()));
      } else {
        ImGui::ColorPicker4(getLabel().c_str(), glm::value_ptr(*ValueObservableElement<T>::getValueAddress()));
      }
    }
    if (oldValue != ValueObservableElement<T>::getValue()) { ValueObservableElement<T>::notifyValueChanged(); }
  }
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_COLORCHOOSER_H
