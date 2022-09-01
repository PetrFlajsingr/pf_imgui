//
// Created by xflajs00 on 11.11.2021.
//

#include "Spinner.h"
#include <pf_imgui/details/Spinner.h>
#include <string>

namespace pf::ui::ig {

Spinner::Spinner(Spinner::Config &&config)
    : ItemElement(std::string{config.name.value}), radius(config.radius), thickness(config.thickness) {}

Spinner::Spinner(const std::string &elementName, float spinnerRadius, int spinnerThickness)
    : ItemElement(elementName), radius(spinnerRadius), thickness(spinnerThickness) {}

void Spinner::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  ImGui::Spinner(getName().c_str(), radius, thickness);
}

}  // namespace pf::ui::ig
