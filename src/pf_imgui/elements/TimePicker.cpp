//
// Created by xflajs00 on 11.04.2022.
//

#include "TimePicker.h"

namespace pf::ui::ig {

TimePicker::TimePicker(TimePicker::Config &&config)
    : TimePicker(std::string{config.name.value}, config.label, config.value,
                 config.persistent ? Persistent::Yes : Persistent::No) {}

TimePicker::TimePicker(const std::string &elementName, const std::string &labelText, pf::ui::ig::TimeOfDay initialValue,
                       pf::ui::ig::Persistent persistent)
    : ItemElement(elementName), Savable(persistent), label(labelText), time(initialValue) {
  updateInnerValues();
  time.addListener([this](auto) { updateInnerValues(); });
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
  ImGui::Text("%s", label->get().c_str());
  ImGui::EndGroup();
}

void TimePicker::updateInnerValues() {
  hours = static_cast<int>(duration_cast<std::chrono::hours>(time->to_duration()).count());
  minutes = static_cast<int>(duration_cast<std::chrono::minutes>(time->to_duration()).count() % 60);
  seconds = static_cast<int>(duration_cast<std::chrono::seconds>(time->to_duration()).count() % 60);
}

toml::table TimePicker::toToml() const {
  return toml::table{{"hours", time->hours().count()},
                     {"minutes", time->minutes().count() % 60},
                     {"seconds", time->seconds().count() % 60}};
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
    *time.modify() = TimeOfDay{std::chrono::hh_mm_ss<std::chrono::seconds>{newHours + newMinutes + newSeconds}};
  }
}

void TimePicker::setValue(const TimeOfDay &newValue) {
  *time.modify() = newValue;
}

const TimeOfDay &TimePicker::getValue() const { return *time; }

Subscription TimePicker::addValueListenerImpl(std::function<void(const TimeOfDay &)> listener) {
  return time.addListener(std::move(listener));
}

void TimePicker::inputChanged() {
  const auto inputTime = TimeOfDay{std::chrono::hours{hours} + std::chrono::minutes{minutes} + std::chrono::seconds{seconds}};
  *time.modify() = inputTime;
}

}  // namespace pf::ui::ig