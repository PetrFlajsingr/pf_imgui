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
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <string>

namespace pf::ui::ig {

// TODO:
// ImGuiColorEditFlags_NoSmallPreview
// ImGuiColorEditFlags_NoInputs
// ImGuiColorEditFlags_NoLabel
// ImGuiColorEditFlags_NoSidePreview
//

/**
 * @brief An element allowing for selecting a color intuitively.
 *
 * It can do either RGB or RGBA colors based on the T parameter.
 *
 * @tparam Type type of the chooser
 *
 */
template<ColorChooserType Type, ColorChooserFormat Format>
class PF_IMGUI_EXPORT ColorChooser : public ItemElement,
                                     public ValueObservable<Color>,
                                     public Savable,
                                     public DragSource<Color>,
                                     public DropTarget<Color> {
 public:
  /**
   * @brief Struct for construction of ColorChooser.
   */
  struct Config {
    using Parent = ColorChooser;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to interactable parts */
    Color value = Color::White;       /*!< Initial value */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct ColorChooser
   * @param config construction args @see ColorChooser::Config
   */
  explicit ColorChooser(Config &&config);
  /**
   * Construct ColorChooser.
   * @param elementName ID of the element
   * @param labelText label next to the intractable parts
   * @param initialValue starting value
   * @param persistent allow state saving to disk
   */
  ColorChooser(const std::string &elementName, const std::string &labelText, Color initialValue = Color::White,
               Persistent persistent = Persistent::No);

  /**
   * Check if picker appearing on color btn click is enabled.
   * @return
   */
  [[nodiscard]] bool isPickerEnabled() const { return pickerEnabled; }
  /**
   * Enable/disable picker
   * @param pickerEnabled
   */
  void setPickerEnabled(bool newValue) { pickerEnabled = newValue; }

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  void setValue(const Color &newValue) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;

 protected:
  void renderImpl() override;

 private:
  bool pickerEnabled = true;
  std::conditional_t<Format == ColorChooserFormat::RGB, glm::vec3, glm::vec4> valueStorage;
};

template<ColorChooserType Type, ColorChooserFormat Format>
ColorChooser<Type, Format>::ColorChooser(ColorChooser::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource(false), DropTarget(false),
      label(std::string{config.label.value}) {
  setValue(config.value);
}

template<ColorChooserType Type, ColorChooserFormat Format>
ColorChooser<Type, Format>::ColorChooser(const std::string &elementName, const std::string &labelText,
                                         Color initialValue, Persistent persistent)
    : ItemElement(elementName), ValueObservable(initialValue), Savable(persistent), DragSource(false),
      DropTarget(false), label(labelText) {
  setValue(initialValue);
}

template<ColorChooserType Type, ColorChooserFormat Format>
toml::table ColorChooser<Type, Format>::toToml() const {
  const auto val = ValueObservable::getValue();
  return toml::table{{"color", static_cast<std::uint32_t>(val)}};
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("color"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_integer(); newVal != nullptr) {
      ValueObservable::setValueAndNotifyIfChanged(Color{static_cast<ImU32>(newVal->get())});
    }
  }
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::setValue(const Color &newValue) {
  valueStorage.r = newValue.red();
  valueStorage.g = newValue.green();
  valueStorage.b = newValue.blue();
  if constexpr (Format == ColorChooserFormat::RGBA) { valueStorage.a = newValue.alpha(); }
  ValueObservable::setValue(newValue);
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto flags = pickerEnabled ? ImGuiColorEditFlags{} : ImGuiColorEditFlags_NoPicker;
  auto valueChanged = false;
  if constexpr (Type == ColorChooserType::Edit) {
    if constexpr (Format == ColorChooserFormat::RGB) {
      valueChanged = ImGui::ColorEdit3(label->get().c_str(), glm::value_ptr(valueStorage), flags);
    } else {
      valueChanged = ImGui::ColorEdit4(label->get().c_str(), glm::value_ptr(valueStorage), flags);
    }
  } else {
    if constexpr (Format == ColorChooserFormat::RGB) {
      valueChanged = ImGui::ColorPicker3(label->get().c_str(), glm::value_ptr(valueStorage), flags);
    } else {
      valueChanged = ImGui::ColorPicker4(label->get().c_str(), glm::value_ptr(valueStorage), flags);
    }
  }
  DragSource::drag(ValueObservable::getValue());
  if (auto drop = DropTarget::dropAccept(); drop.has_value()) {
    ValueObservable::setValueAndNotifyIfChanged(*drop);
    return;
  }
  if (valueChanged) {
    if constexpr (Format == ColorChooserFormat::RGB) {
      ValueObservable::setValueAndNotifyIfChanged(Color::RGB(valueStorage.r, valueStorage.g, valueStorage.b));
    } else {
      ValueObservable::setValueAndNotifyIfChanged(
          Color::RGB(valueStorage.r, valueStorage.g, valueStorage.b, valueStorage.a));
    }
  }
}

/**
 * @brief Convenience alias for ColorChooser as picker.
 */
template<ColorChooserFormat Format>
using ColorPicker = ColorChooser<ColorChooserType::Picker, Format>;

/**
 * @brief Convenience alias for ColorChooser as edit.
 */
template<ColorChooserFormat Format>
using ColorEdit = ColorChooser<ColorChooserType::Edit, Format>;

extern template class ColorChooser<ColorChooserType::Edit, ColorChooserFormat::RGB>;
extern template class ColorChooser<ColorChooserType::Edit, ColorChooserFormat::RGBA>;
extern template class ColorChooser<ColorChooserType::Picker, ColorChooserFormat::RGB>;
extern template class ColorChooser<ColorChooserType::Picker, ColorChooserFormat::RGBA>;

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_COLORCHOOSER_H
