/**
 * @file TimePicker.h
 * @brief TimePicker element.
 * @author Petr Flajšingr
 * @date 11.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_TIMEPICKER_H
#define PF_IMGUI_ELEMENTS_TIMEPICKER_H

#include <chrono>
#include <pf_common/Explicit.h>
#include <pf_imgui/elements/SpinInput.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

using TimeOfDay = std::chrono::hh_mm_ss<std::chrono::seconds>;
namespace details {
struct TimeOfDayComparator {
  [[nodiscard]] inline bool operator()(TimeOfDay lhs, TimeOfDay rhs) const {
    return lhs.to_duration() == rhs.to_duration();
  }
};
}  // namespace details

// TODO: styles
/**
 * @brief Simple HH MM SS 24 time picker.
 */
class TimePicker : public ItemElement,
                   public Labellable,
                   public ValueObservable<TimeOfDay, details::TimeOfDayComparator>,
                   public Savable {
 public:
  /**
   * @brief Construction config for TimePicker.
   */
  struct Config {
    using Parent = TimePicker;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<std::string> label;     /*!< Text rendered next to the input */
    TimeOfDay value{};               /*!< Initial value */
    bool persistent = false;         /*!< Enable disk state saving */
  };
  /**
   * Construct TimePicker
   * @param config construction args @see TimePicker::Config
   */
  explicit TimePicker(Config &&config);
  /**
   * Construct TimePicker
   * @param name unique name of the element
   * @param label text rendered next to the input
   * @param value initial value
   * @param persistent enable disk state saving
   */
  TimePicker(const std::string &name, const std::string &label, TimeOfDay value,
             Persistent persistent = Persistent::No);

  void setValue(const TimeOfDay &newValue) override;

  toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  void inputChanged();

  int hours;
  int minutes;
  int seconds;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_TIMEPICKER_H
