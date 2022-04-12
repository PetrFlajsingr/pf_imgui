//
// Created by xflajs00 on 11.04.2022.
//

#include "TimePicker.h"

namespace pf::ui::ig {

TimePicker::TimePicker(TimePicker::Config &&config)
    : TimePicker(std::string{config.name}, config.label, config.value,
                 config.persistent ? Persistent::Yes : Persistent::No) {}

TimePicker::TimePicker(const std::string &name, const std::string &label, pf::ui::ig::TimeOfDay value,
                       pf::ui::ig::Persistent persistent)
    : Element(name), Labellable(label), ValueObservable(value), Savable(persistent),
      spinH("spinH", "", 0, 23, static_cast<int>(duration_cast<std::chrono::hours>(value.to_duration()).count())),
      spinM("spinM", "", 0, 59,
            static_cast<int>(duration_cast<std::chrono::minutes>(value.to_duration()).count() % 60)),
      spinS("spinS", "", 0, 59,
            static_cast<int>(duration_cast<std::chrono::seconds>(value.to_duration()).count() % 60)) {
  spinH.addValueListener([this](auto hours) {
    const auto time = getValue().to_duration();
    const auto timeMmSs = time - duration_cast<std::chrono::hours>(time);
    const auto newTime = TimeOfDay{std::chrono::hh_mm_ss{timeMmSs + std::chrono::hours{hours}}};
    setValueAndNotifyIfChanged(newTime);
  });
  spinM.addValueListener([this](auto minutes) {
    const auto time = getValue().to_duration();
    const auto timeHhSs = time - duration_cast<std::chrono::minutes>(time) % 60;
    const auto newTime = TimeOfDay{std::chrono::hh_mm_ss{timeHhSs + std::chrono::minutes{minutes}}};
    setValueAndNotifyIfChanged(newTime);
  });
  spinS.addValueListener([this](auto seconds) {
    const auto time = getValue().to_duration();
    const auto timeHhMm = time - duration_cast<std::chrono::seconds>(time) % 60;
    const auto newTime = TimeOfDay{std::chrono::hh_mm_ss{timeHhMm + std::chrono::seconds{seconds}}};
    setValueAndNotifyIfChanged(newTime);
  });
}

void TimePicker::renderImpl() {
  {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    pf::RAII popStyleVar{[] { ImGui::PopStyleVar(); }};
    ImGui::SetNextItemWidth(65);
    spinH.render();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(65);
    spinM.render();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(65);
    spinS.render();
  }
  ImGui::SameLine();
  ImGui::Text("%s", getLabel().c_str());
}

void TimePicker::unserialize_impl(const toml::table &src) {
  using namespace std::chrono;
  auto partsFound = 0;
  hours newHours{};
  minutes newMinutes{};
  seconds newSeconds{};
  if (auto yearIter = src.find("hours"); yearIter != src.end()) {
    if (auto newHoursVal = yearIter->second.as_integer(); newHoursVal != nullptr) {
      newHours = hours{static_cast<unsigned int>(newHoursVal->get())};
      ++partsFound;
    }
  }
  if (auto monthIter = src.find("minutes"); monthIter != src.end()) {
    if (auto newMinutesVal = monthIter->second.as_integer(); newMinutesVal != nullptr) {
      newMinutes = minutes{static_cast<unsigned int>(newMinutesVal->get())};
      ++partsFound;
    }
  }
  if (auto dayIter = src.find("seconds"); dayIter != src.end()) {
    if (auto newSecondsVal = dayIter->second.as_integer(); newSecondsVal != nullptr) {
      newSeconds = seconds{static_cast<unsigned int>(newSecondsVal->get())};
      ++partsFound;
    }
  }

  if (partsFound == 3) { setValue(TimeOfDay{hh_mm_ss<seconds>{newHours + newMinutes + newSeconds}}); }
}

toml::table TimePicker::serialize_impl() const {
  return toml::table{{"hours", getValue().hours().count()},
                     {"minutes", getValue().minutes().count() % 60},
                     {"seconds", getValue().seconds().count() % 60}};
}

void TimePicker::setValue(const pf::ui::ig::TimeOfDay &newValue) {
  spinH.setValue(static_cast<int>(duration_cast<std::chrono::hours>(newValue.to_duration()).count()));
  spinM.setValue(static_cast<int>(duration_cast<std::chrono::minutes>(newValue.to_duration()).count() % 60));
  spinS.setValue(static_cast<int>(duration_cast<std::chrono::seconds>(newValue.to_duration()).count() % 60));
  ValueObservable::setValue(newValue);
}

}  // namespace pf::ui::ig