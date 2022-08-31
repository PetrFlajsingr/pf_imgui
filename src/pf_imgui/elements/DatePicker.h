/**
 * @file DatePicker.h
 * @brief Date picker element.
 * @author Petr Flajšingr
 * @date 14.02.22
 */

#ifndef PF_IMGUI_ELEMENTS_DATEPICKER_H
#define PF_IMGUI_ELEMENTS_DATEPICKER_H

#include <chrono>
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/Label.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

// todo date format and other args
// todo crashes on invalid dates
// TODO: styles
/**
 * @brief Date picker element.
 */
class DatePicker : public ItemElement,
                   public ValueObservable<std::chrono::year_month_day>,
                   public Savable {
 public:
  /**
   * @brief Construction config for DatePicker
   */
  struct Config {
    using Parent = DatePicker;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    std::chrono::year_month_day value = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())}; /*!< Initial value of the element */
    bool persistent = false;                                                      /*!< Allow state saving to disk */
  };
  /**
   * Construct DatePicker
   * @param config construction args @see DatePicker::Config
   */
  explicit DatePicker(Config &&config);

  /**
   * Construct DatePicker
   * @param name unique name of the element
   * @param label text rendered next to the element
   * @param value initial value
   * @param persistent allow state saving to disk
   */
  DatePicker(const std::string &name, const std::string &label,
             std::chrono::year_month_day value =
                 std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())},
             Persistent persistent = Persistent::No);

  void setValue(const std::chrono::year_month_day &newValue) override;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Font font = Font::Default();
  Label label;

 protected:
  void renderImpl() override;

 private:
  std::unique_ptr<tm> rawTime;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_DATEPICKER_H
