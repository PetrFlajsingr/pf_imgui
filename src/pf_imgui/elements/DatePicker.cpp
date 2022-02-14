//
// Created by petr.flajsingr on 2/14/2022.
//

#include "DatePicker.h"
#include "imguidatechooser.h"
#include <time.h>

namespace pf::ui::ig {

DatePicker::DatePicker(DatePicker::Config &&config)
    : Element(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable(config.value),
      Savable(config.persistent), rawTime(std::make_unique<tm>()) {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(config.value.day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(config.value.month()) - 1);
  rawTime->tm_year = static_cast<int>(config.value.year()) - 1900;
}

DatePicker::DatePicker(const std::string &name, const std::string &label, std::chrono::year_month_day value,
                       Persistent persistent)
    : Element(name), Labellable(label), ValueObservable(value), Savable(persistent), rawTime(std::make_unique<tm>()) {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(value.day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(value.month()) - 1);
  rawTime->tm_year = static_cast<int>(value.year()) - 1900;
}

void DatePicker::renderImpl() {
  using namespace std::chrono;
  if (ImGui::DateChooser(getLabel().c_str(), *rawTime)) {
    const auto newDate =
        year_month_day{year{rawTime->tm_year + 1900}, month{static_cast<unsigned int>(rawTime->tm_mon + 1)},
                       day{static_cast<unsigned int>(rawTime->tm_mday)}};
    setValueAndNotifyIfChanged(newDate);
  }
}

}  // namespace pf::ui::ig
