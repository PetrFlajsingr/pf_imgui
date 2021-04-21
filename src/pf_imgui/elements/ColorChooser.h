/**
 * @file ColorChooser.h
 * @brief ColorChooser element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_COLORCHOOSER_H
#define PF_IMGUI_ELEMENTS_COLORCHOOSER_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <toml++/toml_node_view.h>

namespace pf::ui::ig {

/**
 * @brief An element allowing for selecting a color intuitively.
 *
 * It can do either RGB or RGBA colors based on the T parameter.
 *
 * @tparam Type type of the chooser
 * @tparam T inner value of the chooser
 */
template<ColorChooserType Type, OneOf<glm::vec3, glm::vec4> T>
class PF_IMGUI_EXPORT ColorChooser : public ItemElement,
                                     public Labellable,
                                     public ValueObservable<T>,
                                     public Savable,
                                     public DragSource<T>,
                                     public DropTarget<T> {
 public:
  /**
   * Construct ColorChooser.
   * @param elementName ID of the element
   * @param label label next to the intractable parts
   * @param persistent allow state saving to disk
   * @param value starting value
   */
  ColorChooser(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
               T value = T{})
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false) {}

  /**
   * Check if picker appearing on collor btn click is enabled.
   * @return
   */
  [[nodiscard]] bool isPickerEnabled() const { return pickerEnabled; }
  /**
   * Enable/disable picker
   * @param pickerEnabled
   */
  void setPickerEnabled(bool enabled) { ColorChooser::pickerEnabled = enabled; }

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
    auto flags = pickerEnabled ? ImGuiColorEditFlags{} : ImGuiColorEditFlags_NoPicker;
    auto valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    if constexpr (Type == ColorChooserType::Edit) {
      if constexpr (std::same_as<glm::vec3, T>) {
        valueChanged =
            ImGui::ColorEdit3(getLabel().c_str(), glm::value_ptr(*address), flags);
      } else {
        valueChanged =
            ImGui::ColorEdit4(getLabel().c_str(), glm::value_ptr(*address), flags);
      }
    } else {
      if constexpr (std::same_as<glm::vec3, T>) {
        valueChanged =
            ImGui::ColorPicker3(getLabel().c_str(), glm::value_ptr(*address), flags);
      } else {
        valueChanged =
            ImGui::ColorPicker4(getLabel().c_str(), glm::value_ptr(*address), flags);
      }
    }
    DragSource<T>::drag(address);
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  bool pickerEnabled = true;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_COLORCHOOSER_H
