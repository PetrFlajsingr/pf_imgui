//
// Created by petr.flajsingr on 2/14/2022.
//

#include "DatePicker.h"
#include "imguidatechooser.h"
#include <ctime>

namespace pf::ui::ig {

DatePicker::DatePicker(DatePicker::Config &&config)
    : ItemElement(std::string{config.name.value}), ValueObservable(config.value),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), label(std::string{config.label.value}),
      rawTime(std::make_unique<tm>()) {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(config.value.day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(config.value.month()) - 1);
  rawTime->tm_year = static_cast<int>(config.value.year()) - 1900;
}

DatePicker::DatePicker(const std::string &elementName, const std::string &labelText,
                       std::chrono::year_month_day initialValue, Persistent persistent)
    : ItemElement(elementName), ValueObservable(initialValue), Savable(persistent), label(labelText),
      rawTime(std::make_unique<tm>()) {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(initialValue.day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(initialValue.month()) - 1);
  rawTime->tm_year = static_cast<int>(initialValue.year()) - 1900;
}

void DatePicker::setValue(const std::chrono::year_month_day &newValue) {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(newValue.day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(newValue.month()) - 1);
  rawTime->tm_year = static_cast<int>(newValue.year()) - 1900;
  ValueObservable::setValue(newValue);
}

toml::table DatePicker::toToml() const {
  return toml::table{
      {"year", static_cast<int>(getValue().year())},
      {"month", static_cast<unsigned int>(getValue().month())},
      {"day", static_cast<unsigned int>(getValue().day())},
  };
}

void DatePicker::setFromToml(const toml::table &src) {
  using namespace std::chrono;
  auto partsFound = 0;
  year newYear{};
  month newMonth{};
  day newDay{};
  if (auto yearIter = src.find("year"); yearIter != src.end()) {
    if (auto newYearVal = yearIter->second.as_integer(); newYearVal != nullptr) {
      newYear = year{static_cast<int>(newYearVal->get())};
      ++partsFound;
    }
  }
  if (auto monthIter = src.find("month"); monthIter != src.end()) {
    if (auto newMonthVal = monthIter->second.as_integer(); newMonthVal != nullptr) {
      newMonth = month{static_cast<unsigned int>(newMonthVal->get())};
      ++partsFound;
    }
  }
  if (auto dayIter = src.find("day"); dayIter != src.end()) {
    if (auto newDayVal = dayIter->second.as_integer(); newDayVal != nullptr) {
      newDay = day{static_cast<unsigned int>(newDayVal->get())};
      ++partsFound;
    }
  }

  if (partsFound == 3) { setValue(year_month_day{newYear, newMonth, newDay}); }
}

void DatePicker::renderImpl() {
  using namespace std::chrono;
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (ImGui::DateChooser(label.get().c_str(), *rawTime)) {
    const auto newDate =
        year_month_day{year{rawTime->tm_year + 1900}, month{static_cast<unsigned int>(rawTime->tm_mon + 1)},
                       day{static_cast<unsigned int>(rawTime->tm_mday)}};
    setValueAndNotifyIfChanged(newDate);
  }
}

}  // namespace pf::ui::ig
