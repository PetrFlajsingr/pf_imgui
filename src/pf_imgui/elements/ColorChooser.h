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
#include <pf_common/Explicit.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
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
                                     public ValueContainer<Color>,
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
  ColorChooser(std::string_view elementName, std::string_view labelText, Color initialValue = Color::White,
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
  [[nodiscard]] const Color &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const Color &)> listener) override;

 public:
  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget, ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Property<Label> label;
  Property<Color> chosenColor;

 protected:
  void renderImpl() override;

 private:
  void updateValueStorage();
  bool pickerEnabled = true;
  std::conditional_t<Format == ColorChooserFormat::RGB, glm::vec3, glm::vec4> valueStorage;
};

template<ColorChooserType Type, ColorChooserFormat Format>
ColorChooser<Type, Format>::ColorChooser(ColorChooser::Config &&config)
    : ColorChooser(config.name, config.label, config.value, config.persistent ? Persistent::Yes : Persistent::No) {}

template<ColorChooserType Type, ColorChooserFormat Format>
ColorChooser<Type, Format>::ColorChooser(std::string_view elementName, std::string_view labelText, Color initialValue,
                                         Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource(false), DropTarget(false), label(std::string{labelText}),
      chosenColor(initialValue) {
  updateValueStorage();
  chosenColor.addListener([this](auto) { updateValueStorage(); });
}

template<ColorChooserType Type, ColorChooserFormat Format>
toml::table ColorChooser<Type, Format>::toToml() const {
  return toml::table{{"color", static_cast<std::uint32_t>(*chosenColor)}};
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("color"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_integer(); newVal != nullptr) {
      *chosenColor.modify() = Color{static_cast<ImU32>(newVal->get())};
    }
  }
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
  DragSource::drag(*chosenColor);
  if (auto drop = DropTarget::dropAccept(); drop.has_value()) {
    *chosenColor.modify() = *drop;
    return;
  }
  if (valueChanged) {
    if constexpr (Format == ColorChooserFormat::RGB) {
      *chosenColor.modify() = Color::RGB(valueStorage.r, valueStorage.g, valueStorage.b);
    } else {
      *chosenColor.modify() = Color::RGB(valueStorage.r, valueStorage.g, valueStorage.b, valueStorage.a);
    }
  }
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::updateValueStorage() {
  valueStorage.r = chosenColor->red();
  valueStorage.g = chosenColor->green();
  valueStorage.b = chosenColor->blue();
  if constexpr (Format == ColorChooserFormat::RGBA) { valueStorage.a = chosenColor->alpha(); }
}

template<ColorChooserType Type, ColorChooserFormat Format>
void ColorChooser<Type, Format>::setValue(const Color &newValue) {
  *chosenColor.modify() = newValue;
}

template<ColorChooserType Type, ColorChooserFormat Format>
const Color &ColorChooser<Type, Format>::getValue() const {
  return *chosenColor;
}

template<ColorChooserType Type, ColorChooserFormat Format>
Subscription ColorChooser<Type, Format>::addValueListenerImpl(std::function<void(const Color &)> listener) {
  return chosenColor.addListener(std::move(listener));
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
