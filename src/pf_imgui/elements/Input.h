//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_INPUT_H
#define PF_IMGUI_ELEMENTS_INPUT_H

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
#define IMGUI_INPUT_STEP_TYPE_LIST float, double, int
#define IMGUI_INPUT_FLOAT_TYPE_LIST float, glm::vec2, glm::vec3, glm::vec4
#define IMGUI_INPUT_DOUBLE_TYPE_LIST double
#define IMGUI_INPUT_INT_TYPE_LIST int, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_INPUT_GLM_TYPE_LIST glm::vec2, glm::vec3, glm::vec4, glm::ivec2, glm::ivec3, glm::ivec4
#define IMGUI_INPUT_TYPE_LIST IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_INT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST

/**
 * Underlying type of supported types.
 */
template<OneOf<IMGUI_INPUT_TYPE_LIST> T>
using InputUnderlyingType = std::conditional_t<OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST>, float,
                                               std::conditional_t<OneOf<T, IMGUI_INPUT_INT_TYPE_LIST>, int, double>>;
/**
 * Storage structure for different underlying types.
 * @tparam T underlying type
 */
template<typename T>
struct InputData {};
/**
 * @brief Storage structure for integer based types.
 */
template<>
struct InputData<int> {
  int step;
  int fastStep;
};
/**
 * @brief Storage structure for float based types.
 */
template<>
struct InputData<float> {
  float step;
  float fastStep;
  static constexpr const char *defaultFormat() { return "%.3f"; }
};
/**
 * @brief Storage structure for double based types.
 */
template<>
struct InputData<double> {
  double step;
  double fastStep;
  static constexpr const char *defaultFormat() { return "%.6f"; }
};
/**
 * Types which have no formatting but have a step.
 * @tparam T
 */
template<typename T>
concept UnformattedWithStep =
    OneOf<T, IMGUI_INPUT_STEP_TYPE_LIST> && !OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST>;
/**
 * Types which have no formatting and no step.
 * @tparam T
 */
template<typename T>
concept UnformattedWithoutStep =
    !OneOf<T, IMGUI_INPUT_STEP_TYPE_LIST> && !OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST>;
/**
 * Types which have formatting and a step.
 * @tparam T
 */
template<typename T>
concept FormattedWithStep =
    OneOf<T, IMGUI_INPUT_STEP_TYPE_LIST> && OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST>;
/**
 * Types which have formatting but have no step.
 * @tparam T
 */
template<typename T>
concept FormattedWithoutStep =
    !OneOf<T, IMGUI_INPUT_STEP_TYPE_LIST> && OneOf<T, IMGUI_INPUT_FLOAT_TYPE_LIST, IMGUI_INPUT_DOUBLE_TYPE_LIST>;
}// namespace details

/**
 * @brief Keyboard input for numeric types.
 *
 * Type of the Input is based on underlying types. There is a separate input for each scalar part of type T.
 *
 * @tparam T Underlying type
 */
template<OneOf<IMGUI_INPUT_TYPE_LIST> T>
class PF_IMGUI_EXPORT Input : public ItemElement, public Labellable, public ValueObservable<T>, public Savable {
  details::InputData<details::InputUnderlyingType<T>> data;

 public:
  /**
   * Construct Input.
   * @param elementName ID of the input
   * @param label text drawn next to the input
   * @param st step
   * @param fStep fast step
   * @param persistent enable state saving to disk
   * @param value starting value
   */
  Input(const std::string &elementName, const std::string &label, T st = 0, T fStep = 0,
        Persistent persistent = Persistent::No, T value = T{}) requires details::UnformattedWithStep<T>
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value), Savable(persistent), data(st, fStep) {}

  /**
   * Construct Input.
   * @param elementName ID of the input
   * @param label text drawn next to the input
   * @param st step
   * @param fStep fast step
   * @param format format for printing underlying float value
   * @param persistent enable state saving to disk
   * @param value starting value
   */
  Input(const std::string &elementName, const std::string &label, T st = 0, T fStep = 0,
        std::string format = decltype(data)::defaultFormat(), Persistent persistent = Persistent::No,
        T value = T{}) requires details::FormattedWithStep<T> : ItemElement(elementName),
                                                                Labellable(label),
                                                                ValueObservable<T>(value),
                                                                Savable(persistent),
                                                                format(std::move(format)),
                                                                data(st, fStep) {}

  /**
   * Construct Input.
   * @param elementName ID of the input
   * @param label text drawn next to the input
   * @param persistent enable state saving to disk
   * @param value starting value
   */
  Input(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
        T value = T{}) requires details::UnformattedWithoutStep<T>
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value), Savable(persistent) {}

  /**
   * Construct Input.
   * @param elementName ID of the input
   * @param label text drawn next to the input
   * @param persistent enable state saving to disk
   * @param format format for printing underlying float value
   * @param value starting value
   */
  Input(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
        std::string format = decltype(data)::defaultFormat(), T value = T{}) requires details::FormattedWithoutStep<T>
      : ItemElement(elementName),
        Labellable(label),
        ValueObservable<T>(value),
        Savable(persistent),
        format(std::move(format)) {}

 protected:
  void unserialize_impl(const toml::table &src) override {
    if constexpr (OneOf<T, IMGUI_INPUT_GLM_TYPE_LIST>) {
      const auto tomlVec = src["value"].as_array();
      const auto vec = deserializeGlmVec<T>(*tomlVec);
      ValueObservable<T>::setValueAndNotifyIfChanged(vec);
    } else {
      ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
    }
  }

  toml::table serialize_impl() override {
    const auto value = ValueObservable<T>::getValue();
    if constexpr (OneOf<T, IMGUI_INPUT_GLM_TYPE_LIST>) {
      return toml::table{{{"value", serializeGlmVec(value)}}};
    } else {
      return toml::table{{{"value", value}}};
    }
  }

  void renderImpl() override {
    auto valueChanged = false;
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::InputFloat(getLabel().c_str(), ValueObservable<T>::getValueAddress(), data.step,
                                       data.fastStep, format.c_str());
    }
    if constexpr (std::same_as<T, glm::vec2>) {
      valueChanged = ImGui::InputFloat2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, glm::vec3>) {
      valueChanged = ImGui::InputFloat3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, glm::vec4>) {
      valueChanged = ImGui::InputFloat4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged =
          ImGui::InputInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), data.step, data.fastStep);
    }
    if constexpr (std::same_as<T, glm::ivec2>) {
      valueChanged = ImGui::InputInt2(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, glm::ivec3>) {
      valueChanged = ImGui::InputInt3(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, glm::ivec4>) {
      valueChanged = ImGui::InputInt4(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()));
    }
    if constexpr (std::same_as<T, double>) {
      valueChanged = ImGui::InputDouble(getLabel().c_str(), glm::value_ptr(*ValueObservable<T>::getValueAddress()),
                                        data.step, data.fastStep, format.c_str());
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  std::string format;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_INPUT_H
