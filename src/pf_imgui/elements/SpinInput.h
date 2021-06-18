/**
 * @file SpinInput.h
 * @brief Spinner element.
 * @author Petr Flajšingr
 * @date 23.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_SPININPUT_H
#define PF_IMGUI_ELEMENTS_SPININPUT_H

#include <pf_imgui/details/Spin.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

/**
 * @brief Spinner for numeric types.
 * @tparam T inner type
 * @todo: format for float
 */
template<OneOf<int, float> T>
class SpinInput
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  /**
   * Construct SpinInput.
   * @param elementName ID of the element
   * @param label texte rendered next to the element
   * @param value starting value
   * @param step spin step
   * @param stepFast fast spin step
   * @param persistent enable/disable state saving od disk
   */
  SpinInput(const std::string &elementName, const std::string &label, T minVal, T maxVal, T value = T{}, T step = T{1},
            const T &stepFast = T{100}, Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), step(step), stepFast(stepFast), min(minVal),
        max(maxVal) {}

  [[nodiscard]] const T &getMin() const { return min; }
  void setMin(const T &minVal) { min = minVal; }
  [[nodiscard]] const T &getMax() const { return max; }
  void setMax(const T &maxVal) { max = maxVal; }

 protected:
  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    auto valueChanged = false;
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::SpinInt(getLabel().c_str(), ValueObservable<T>::getValueAddress(), step, stepFast);
    }
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::SpinFloat(getLabel().c_str(), ValueObservable<T>::getValueAddress(), step, stepFast);
    }
    if (valueChanged) {
      ValueObservable<T>::setValueInner(std::clamp(ValueObservable<T>::getValue(), min, max));
      ValueObservable<T>::notifyValueChanged();
    }
    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      const auto value = std::clamp(*drop, min, max);
      ValueObservable<T>::setValueAndNotifyIfChanged(value);
      return;
    }
  }

  void unserialize_impl(const toml::table &src) override {
    ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
  }

  toml::table serialize_impl() override { return toml::table{{{"value", ValueObservable<T>::getValue()}}}; }

 private:
  T step;
  T stepFast;
  T min;
  T max;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_SPININPUT_H
