//
// Created by xflajs00 on 11.04.2022.
//

#include "TimePicker.h"

namespace pf::ui::ig {

TimePicker::TimePicker(TimePicker::Config &&config)
    : TimePicker(std::string{config.name.value}, config.label, config.value,
                 config.persistent ? Persistent::Yes : Persistent::No) {
  setValue(config.value);
}

TimePicker::TimePicker(const std::string &elementName, const std::string &labelText, pf::ui::ig::TimeOfDay initialValue,
                       pf::ui::ig::Persistent persistent)
    : ItemElement(elementName), ValueObservable(initialValue), Savable(persistent), label(labelText) {
  setValue(initialValue);
}

void TimePicker::renderImpl() {
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::BeginGroup();
  {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    pf::RAII popStyleVar{[] { ImGui::PopStyleVar(); }};
    ImGui::SetNextItemWidth(65);
    if (ImGui::SpinInt("##hours", &hours, 1, 2)) {
      if (hours >= 24) { hours = 0; }
      if (hours < 0) { hours = 23; }
      inputChanged();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(65);
    if (ImGui::SpinInt("##minutes", &minutes, 1, 2)) {
      if (minutes >= 60) { minutes = 0; }
      if (minutes < 0) { minutes = 60; }
      inputChanged();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(65);
    if (ImGui::SpinInt("##seconds", &seconds, 1, 2)) {
      if (seconds >= 60) { seconds = 0; }
      if (seconds < 0) { seconds = 60; }
      inputChanged();
    }
  }
  ImGui::SameLine();
  ImGui::Text("%s", label.get().c_str());
  ImGui::EndGroup();
}

void TimePicker::setValue(const pf::ui::ig::TimeOfDay &newValue) {
  hours = static_cast<int>(duration_cast<std::chrono::hours>(newValue.to_duration()).count());
  minutes = static_cast<int>(duration_cast<std::chrono::minutes>(newValue.to_duration()).count() % 60);
  seconds = static_cast<int>(duration_cast<std::chrono::seconds>(newValue.to_duration()).count() % 60);
  ValueObservable::setValue(newValue);
}

toml::table TimePicker::toToml() const {
  return toml::table{{"hours", getValue().hours().count()},
                     {"minutes", getValue().minutes().count() % 60},
                     {"seconds", getValue().seconds().count() % 60}};
}

void TimePicker::setFromToml(const toml::table &src) {
  auto partsFound = 0;
  std::chrono::hours newHours{};
  std::chrono::minutes newMinutes{};
  std::chrono::seconds newSeconds{};
  if (auto yearIter = src.find("hours"); yearIter != src.end()) {
    if (auto newHoursVal = yearIter->second.as_integer(); newHoursVal != nullptr) {
      newHours = std::chrono::hours{static_cast<unsigned int>(newHoursVal->get())};
      ++partsFound;
    }
  }
  if (auto monthIter = src.find("minutes"); monthIter != src.end()) {
    if (auto newMinutesVal = monthIter->second.as_integer(); newMinutesVal != nullptr) {
      newMinutes = std::chrono::minutes{static_cast<unsigned int>(newMinutesVal->get())};
      ++partsFound;
    }
  }
  if (auto dayIter = src.find("seconds"); dayIter != src.end()) {
    if (auto newSecondsVal = dayIter->second.as_integer(); newSecondsVal != nullptr) {
      newSeconds = std::chrono::seconds{static_cast<unsigned int>(newSecondsVal->get())};
      ++partsFound;
    }
  }

  if (partsFound == 3) {
    setValue(TimeOfDay{std::chrono::hh_mm_ss<std::chrono::seconds>{newHours + newMinutes + newSeconds}});
  }
}

void TimePicker::inputChanged() {
  const auto inputTime = std::chrono::hours{hours} + std::chrono::minutes{minutes} + std::chrono::seconds{seconds};
  setValueAndNotifyIfChanged(TimeOfDay{std::chrono::hh_mm_ss<std::chrono::seconds>(inputTime)});
}

}  // namespace pf::ui::ig