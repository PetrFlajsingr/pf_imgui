/**
 * @file Input.h
 * @brief Input element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_INPUT_H
#define PF_IMGUI_ELEMENTS_INPUT_H

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/elements/details/InputDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Keyboard input for numeric types.
 *
 * Type of the Input is based on underlying types. There is a separate input for each scalar part of type T.
 *
 * Supported types are int, float, double, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
 *
 * @tparam T Underlying type
 */
template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
class PF_IMGUI_EXPORT Input : public ItemElement,
                              public ValueContainer<T>,
                              public Savable,
                              public DragSource<T>,
                              public DropTarget<T> {
  using StepType = input_details::UnderlyingType<T>;

 public:
  struct Config {
    using Parent = Input;

    Explicit<std::string_view> name;                        /*!< Unique name of the element */
    Explicit<std::string_view> label;                       /*!< Text rendered next to the input */
    StepType step{static_cast<StepType>(1)};                /*!< Speed of value change */
    StepType fastStep{static_cast<StepType>(10)};           /*!< Fast speed of value change */
    T value{};                                              /*!< Initial value */
    std::string format = input_details::defaultFormat<T>(); /*!< Format string to render value */
    bool persistent = false;                                /*!< Allow state saving to disk */
  };
  /**
   * Construct Input
   * @param config construction args @see Input::Config
   */
  explicit Input(Config &&config);

  /**
   * Construct Input. For the following types: float, double.
   * @param elementName ID of the input
   * @param labelText text drawn next to the input
   * @param st step
   * @param fStep fast step
   * @param initialValue starting value
   * @param persistent enable state saving to disk
   * @param numberFormat format for printing underlying float value
   */
  Input(std::string_view elementName, std::string_view labelText, StepType st = static_cast<StepType>(1),
        StepType fStep = static_cast<StepType>(10), T initialValue = T{}, Persistent persistent = Persistent::No,
        std::string numberFormat = input_details::defaultFormat<T>());

  [[nodiscard]] bool isReadOnly() const { return readOnly; }

  void setReadOnly(bool isReadOnly);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered,
               ColorOf::FrameBackgroundActive, ColorOf::DragDropTarget, ColorOf::TextSelectedBackground,
               ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;
  ObservableProperty<Input, T> value;

  [[nodiscard]] const T &getValue() const override;
  void setValue(const T &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const T &)> listener) override;

  void renderImpl() override;

 private:
  StepType step;
  StepType fastStep;
  std::string format;
  bool readOnly = false;
  ImGuiInputTextFlags flags = {};
};

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
Input<T>::Input(Input::Config &&config)
    : Input(config.name, config.label, config.step, config.fastStep, config.value,
            config.persistent ? Persistent::Yes : Persistent::No, config.format) {}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
Input<T>::Input(std::string_view elementName, std::string_view labelText, Input::StepType st, Input::StepType fStep,
                T initialValue, Persistent persistent, std::string numberFormat)
    : ItemElement(elementName), Savable(persistent), DragSource<T>(false), DropTarget<T>(false),
      label(std::string{labelText}), value(initialValue), step(st), fastStep(fStep), format(std::move(numberFormat)) {}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::setReadOnly(bool isReadOnly) {
  readOnly = isReadOnly;
  if (readOnly) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  } else {
    flags &= ~ImGuiInputTextFlags_ReadOnly;
  }
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
toml::table Input<T>::toToml() const {
  if constexpr (OneOf<T, PF_IMGUI_INPUT_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(*value)}};
  } else {
    return toml::table{{"value", *value}};
  }
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, PF_IMGUI_INPUT_GLM_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
        if (vecValue.has_value()) { *value.modify() = *vecValue; }
      }
    }
  } else {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) { *value.modify() = *newVal; }
    }
  }
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  const auto address = &value.value;

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, PF_IMGUI_INPUT_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else if constexpr (OneOf<T, PF_IMGUI_INPUT_INT_TYPE_LIST>) {
    dataType = ImGuiDataType_S32;
  } else {
    dataType = ImGuiDataType_Double;
  }

  if constexpr (!OneOf<T, PF_IMGUI_INPUT_GLM_TYPE_LIST>) {
    valueChanged = ImGui::InputScalar(label->get().c_str(), dataType, address, &step, &fastStep, format.c_str(), flags);
  } else {
    valueChanged = ImGui::InputScalarN(label->get().c_str(), dataType, glm::value_ptr(*address), T::length(), &step,
                                       &fastStep, format.c_str(), flags);
  }

  DragSource<T>::drag(*value);
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    *value.modify() = *drop;
    return;
  }
  if (valueChanged) { value.triggerListeners(); }
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
const T &Input<T>::getValue() const {
  return *value;
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::setValue(const T &newValue) {
  *value.modify() = newValue;
}

template<OneOf<PF_IMGUI_INPUT_TYPE_LIST, std::string> T>
Subscription Input<T>::addValueListenerImpl(std::function<void(const T &)> listener) {
  return value.addListener(std::move(listener));
}

template<>
class Input<std::string> : public InputText {
 public:
  /**
   * @brief Struct for construction of Input<std::string>.
   */
  struct Config {
    using Parent = Input<std::string>;
    Explicit<std::string_view> name;                     /*!< Unique name of the element */
    Explicit<std::string_view> label;                    /*!< Text rendered next to the input */
    std::string value{};                                 /*!< Initial value */
    TextInputType inputType = TextInputType::SingleLine; /*!< Type of text input */
    std::size_t maxInputLength = 256;                    /*!< Maximum length of input string */
    TextTrigger eventTrigger = TextTrigger::Character;   /*!< Trigger to notify listeners */
    Flags<TextFilter> filters = TextFilter::None;        /*!< Enabled text filters */
    bool persistent = false;                             /*!< Allow state saving to disk */
  };
  /**
   * Construct Input<std::string>
   * @param config construction args @see Input<std::string>::Config
   */
  explicit Input(Config &&config)
      : InputText(InputText::Config{config.name, config.label, config.value, config.inputType, config.maxInputLength,
                                    config.eventTrigger, config.filters, config.persistent}) {}
  /**
   * Construct Input<std::string>.
   * @param elementName ID of the input
   * @param labelText text rendered next to the input
   * @param initialValue starting text in the input
   * @param textInputType singleline or multiline support
   * @param filters character filters for input
   * @param persistent enable state saving to disk
   */
  Input(const std::string &elementName, std::string labelText, const std::string &initialValue = "",
        TextInputType textInputType = TextInputType::SingleLine, std::size_t inputLengthLimit = 256,
        TextTrigger trigger = TextTrigger::Character, const Flags<TextFilter> &filters = TextFilter::None,
        Persistent persistent = Persistent::No)
      : InputText(elementName, std::move(labelText), initialValue, textInputType, inputLengthLimit, trigger, filters,
                  persistent) {}
};

extern template class Input<float>;
extern template class Input<glm::vec2>;
extern template class Input<glm::vec3>;
extern template class Input<glm::vec4>;
extern template class Input<double>;
extern template class Input<int>;
extern template class Input<glm::ivec2>;
extern template class Input<glm::ivec3>;
extern template class Input<glm::ivec4>;

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_INPUT_H
