/**
* @file Input.h
* @brief Input element.
* @author Petr Flajšingr
* @date 31.10.20
*/

#ifndef PF_IMGUI_ELEMENTS_INPUT_H
#define PF_IMGUI_ELEMENTS_INPUT_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <pf_imgui/serialization.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

namespace details {
/**
* Underlying types supporting step.
*/
#define IMGUI_INPUT_STEP_TYPE_LIST float, double, int
/**
* Types using float as underlying value.
*/
#define IMGUI_INPUT_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
/**
* Types using double as underlying value.
*/
#define IMGUI_INPUT_DOUBLE_TYPE_LIST double
/**
* Types using int as underlying value.
*/
#define IMGUI_INPUT_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
/**
* Supported glm types.
*/
#define IMGUI_INPUT_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
/**
* All types supported by Input.
*/
#define IMGUI_INPUT_TYPE_LIST IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_INT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST

/**
* Underlying type of supported types.
*/
template<OneOf<IMGUI_INPUT_TYPE_LIST> T>
using InputUnderlyingType = std::conditional_t<OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST>, float,
                                               std::conditional_t<OneOf<T, IMGUI_INPUT_INT_TYPE_LIST>, int, double>>;
/**
 * Default formatting string for supported types.
 * @tparam T type to based format on
 * @return printf like format for numbers
 */
template<typename T>
constexpr const char *defaultInputFormat() {
  if constexpr (std::same_as<InputUnderlyingType<T>, float>) {
    return "%.3f";
  } else if constexpr (std::same_as<InputUnderlyingType<T>, double>) {
    return "%.6f";
  } else {
    return "%d";
  }
}

}  // namespace details

/**
* @brief Keyboard input for numeric types.
*
* Type of the Input is based on underlying types. There is a separate input for each scalar part of type T.
*
* Supported types are int, float, double, glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
*
* @tparam T Underlying type
*/
template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
class PF_IMGUI_EXPORT Input
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::FrameBackground,
                               style::ColorOf::FrameBackgroundHovered, style::ColorOf::FrameBackgroundActive,
                               style::ColorOf::DragDropTarget, style::ColorOf::TextSelectedBackground,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
  using StepType = details::InputUnderlyingType<T>;

 public:
  struct Config {
    using Parent = Input;

    std::string_view name;                                 /*!< Unique name of the element */
    std::string_view label;                                /*!< Text rendered next to the input */
    StepType step{};                                       /*!< Speed of value change */
    StepType fastStep{};                                   /*!< Fast speed of value change */
    T value{};                                             /*!< Initial value */
    std::string format = details::defaultInputFormat<T>(); /*!< Format string to render value */
    bool persistent = false;                               /*!< Allow state saving to disk */
  };
  /**
   * Construct Input
   * @param config construction args @see Input::Config
   */
  explicit Input(Config &&config);

  /**
  * Construct Input. For the following types: float, double.
  * @param elementName ID of the input
  * @param label text drawn next to the input
  * @param st step
  * @param fStep fast step
  * @param format format for printing underlying float value
  * @param persistent enable state saving to disk
  * @param value starting value
  */
  Input(const std::string &elementName, const std::string &label, StepType st = 0, StepType fStep = 0, T value = T{},
        Persistent persistent = Persistent::No, std::string format = details::defaultInputFormat<T>());

  [[nodiscard]] bool isReadOnly() const { return readOnly; }

  void setReadOnly(bool isReadOnly);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  StepType step;
  StepType fastStep;
  std::string format;
  bool readOnly = false;
  ImGuiInputTextFlags flags = {};
};

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
Input<T>::Input(Input::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable<T>(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<T>(false), DropTarget<T>(false),
      step(config.step), fastStep(config.fastStep), format(std::move(static_cast<std::string>(config.format))) {}

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
Input<T>::Input(const std::string &elementName, const std::string &label, Input::StepType st, Input::StepType fStep,
                T value, Persistent persistent, std::string format)
    : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
      Savable(persistent), DragSource<T>(false), DropTarget<T>(false), step(st), fastStep(fStep),
      format(std::move(format)) {}

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::setReadOnly(bool isReadOnly) {
  readOnly = isReadOnly;
  if (readOnly) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  } else {
    flags &= ~ImGuiInputTextFlags_ReadOnly;
  }
}

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
toml::table Input<T>::toToml() const {
  const auto value = ValueObservable<T>::getValue();
  if constexpr (OneOf<T, IMGUI_INPUT_GLM_TYPE_LIST>) {
    return toml::table{{"value", serializeGlmVec(value)}};
  } else {
    return toml::table{{"value", value}};
  }
}

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::setFromToml(const toml::table &src) {
  if constexpr (OneOf<T, IMGUI_INPUT_GLM_TYPE_LIST>) {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
        const auto vecValue = safeDeserializeGlmVec<T>(*newVal);
        if (vecValue.has_value()) { ValueObservable<T>::setValueAndNotifyIfChanged(vecValue.value()); }
      }
    }
  } else {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) {
        ValueObservable<T>::setValueAndNotifyIfChanged(*newVal);
      }
    }
  }
}

template<OneOf<IMGUI_INPUT_TYPE_LIST, std::string> T>
void Input<T>::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  auto valueChanged = false;
  const auto address = ValueObservable<T>::getValueAddress();

  ImGuiDataType_ dataType;
  if constexpr (OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST>) {
    dataType = ImGuiDataType_Float;
  } else if constexpr (OneOf<T, IMGUI_INPUT_INT_TYPE_LIST>) {
    dataType = ImGuiDataType_S32;
  } else {
    dataType = ImGuiDataType_Double;
  }

  if constexpr (!OneOf<T, IMGUI_INPUT_GLM_TYPE_LIST>) {
    valueChanged = ImGui::InputScalar(getLabel().c_str(), dataType, address, &step, &fastStep, format.c_str(), flags);
  } else {
    valueChanged = ImGui::InputScalarN(getLabel().c_str(), dataType, glm::value_ptr(*address), T::length(), &step,
                                       &fastStep, format.c_str(), flags);
  }

  DragSource<T>::drag(ValueObservable<T>::getValue());
  if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
    ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
    return;
  }
  if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
}

template<>
class Input<std::string> : public InputText {};

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
