/**
 * @file Knob.h
 * @brief Knob element, similar to slider.
 * @author Petr Flajšingr
 * @date 9.5.22
 */

#ifndef PF_IMGUI_ELEMENTS_KNOB_H
#define PF_IMGUI_ELEMENTS_KNOB_H

#include <imgui-knobs.h>
#include <pf_common/concepts/OneOf.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {
/**
 * @brief Type of Knob element.
 */
enum class KnobType {
  Tick = ImGuiKnobVariant_Tick,
  Dot = ImGuiKnobVariant_Dot,
  Wiper = ImGuiKnobVariant_Wiper,
  WiperOnly = ImGuiKnobVariant_WiperOnly,
  WiperDot = ImGuiKnobVariant_WiperDot,
  Stepped = ImGuiKnobVariant_Stepped,
  Space = ImGuiKnobVariant_Space
};

/**
 * @brief Element similar to slider.
 * @tparam T underlying value type
 */
template<OneOf<int, float> T>
class Knob : public Element,
             public Labellable,
             public Resizable,
             public ValueObservable<T>,
             public Savable,
             public ColorCustomizable<style::ColorOf::ButtonActive, style::ColorOf::ButtonHovered,
                                      style::ColorOf::FrameBackground> {
 public:
  /**
   * @brief Construction args for Knob
   */
  struct Config {
    using Parent = Knob;
    std::string_view name;   /*!< Unique name of the element */
    std::string label;       /*!< Label rendered above the element */
    Size size;               /*!< Size of the element */
    KnobType type;           /*!< Type of knob */
    T minValue;              /*!< Min allowed value */
    T maxValue;              /*!< Max allowed value */
    float speed = 1.f;       /*!< Value change speed */
    T value = T{};           /*!< Initial value */
    bool persistent = false; /*!< Enable state saving to disk */
  };
  /**
   * Construct Knob.
   * @param config construction args @see Knob::Config
   */
  explicit Knob(Config &&config)
      : Knob(std::string{config.name}, std::string{config.label}, config.type, config.size, config.minValue,
             config.maxValue, config.speed, config.value, config.persistent ? Persistent::Yes : Persistent::No) {}
  /**
   * Construct Knob
   * @param name unique name of the lement
   * @param label label rendered above the element
   * @param type type of knob
   * @param s size of the element
   * @param min min allowed value
   * @param max max allowed value
   * @param speed value change speed
   * @param value initial value
   * @param persistent enable state saving to disk
   */
  Knob(const std::string &name, const std::string &label, KnobType type, Size s, T min, T max, float speed = 1.f,
       T value = T{}, Persistent persistent = Persistent::No)
      : Element(name), Labellable(label), Resizable(s), ValueObservable<T>(value), Savable(persistent), min(min),
        max(max), speed(speed), type(type) {}

  [[nodiscard]] toml::table toToml() const override { return toml::table{{"value", ValueObservable<T>::getValue()}}; }

  void setFromToml(const toml::table &src) override {
    if (auto newValIter = src.find("value"); newValIter != src.end()) {
      if (auto newVal = newValIter->second.value<T>(); newVal.has_value()) {
        ValueObservable<T>::setValueAndNotifyIfChanged(*newVal);
      }
    }
  }

 protected:
  void renderImpl() override {
    const auto flags = isLabelVisible() ? ImGuiKnobFlags_{} : ImGuiKnobFlags_::ImGuiKnobFlags_NoTitle;
    const auto knobSize = std::min(static_cast<float>(getSize().width), static_cast<float>(getSize().height));
    auto valueChanged = false;
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGuiKnobs::KnobInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), min, max, speed,
                                         nullptr, static_cast<ImGuiKnobVariant>(type), knobSize, flags);
    }
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGuiKnobs::Knob(getLabel().c_str(), ValueObservable<T>::getValueAddress(), min, max, speed,
                                      nullptr, static_cast<ImGuiKnobVariant>(type), knobSize, flags);
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

 private:
  T min;
  T max;
  float speed;
  KnobType type;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_KNOB_H
