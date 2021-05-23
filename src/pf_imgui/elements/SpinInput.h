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
class SpinInput : public ItemElement,
                  public Labellable,
                  public ValueObservable<T>,
                  public Savable,
                  public DragSource<T>,
                  public DropTarget<T> {
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
  SpinInput(const std::string &elementName, const std::string &label, const T &value = T{}, const T &step = T{1},
            const T &stepFast = T{100}, Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable(value), Savable(persistent), DragSource(false),
        DropTarget(false), step(step), stepFast(stepFast) {}

 protected:
  void renderImpl() override {
    const auto oldVal = getValue();
    if constexpr (std::same_as<T, int>) { ImGui::SpinInt(getLabel().c_str(), getValueAddress(), step, stepFast); }
    if constexpr (std::same_as<T, float>) { ImGui::SpinFloat(getLabel().c_str(), getValueAddress(), step, stepFast); }
    if (getValue() != oldVal) { notifyValueChanged(); }
    drag(getValue());
    if (auto drop = dropAccept(); drop.has_value()) {
      setValueAndNotifyIfChanged(*drop);
      return;
    }
  }

  void unserialize_impl(const toml::table &src) override { setValueAndNotifyIfChanged(*src["value"].value<T>()); }

  toml::table serialize_impl() override { return toml::table{{{"value", getValue()}}}; }

 private:
  T step;
  T stepFast;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_SPININPUT_H
