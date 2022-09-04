//
// Created by petr.flajsingr on 2/14/2022.
//

#include "DatePicker.h"
#include "imguidatechooser.h"
#include <ctime>

namespace pf::ui::ig {

DatePicker::DatePicker(DatePicker::Config &&config)
    : ItemElement(std::string{config.name.value}), Savable(config.persistent ? Persistent::Yes : Persistent::No),
      label(std::string{config.label.value}), date(config.value), rawTime(std::make_unique<tm>()) {
  updateRawTime();
  date.addListener([this](auto) { updateRawTime(); });
}

DatePicker::DatePicker(const std::string &elementName, const std::string &labelText,
                       std::chrono::year_month_day initialValue, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(labelText), date(initialValue),
      rawTime(std::make_unique<tm>()) {
  updateRawTime();
  date.addListener([this](auto) { updateRawTime(); });
}

void DatePicker::setValue(const std::chrono::year_month_day &newValue) { *date.modify() = newValue; }

const std::chrono::year_month_day &DatePicker::getValue() const { return *date; }

Subscription DatePicker::addValueListenerImpl(std::function<void(std::chrono::year_month_day)> listener) {
  return date.addListener(std::move(listener));
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
  if (ImGui::DateChooser(label->get().c_str(), *rawTime)) {
    const auto newDate =
        year_month_day{year{rawTime->tm_year + 1900}, month{static_cast<unsigned int>(rawTime->tm_mon + 1)},
                       day{static_cast<unsigned int>(rawTime->tm_mday)}};
    *date.modify() = newDate;
  }
}

void DatePicker::updateRawTime() {
  rawTime->tm_mday = static_cast<int>(static_cast<unsigned int>(date->day()));
  rawTime->tm_mon = static_cast<int>(static_cast<unsigned int>(date->month()) - 1);
  rawTime->tm_year = static_cast<int>(date->year()) - 1900;
}

}  // namespace pf::ui::ig
