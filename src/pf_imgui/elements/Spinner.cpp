//
// Created by xflajs00 on 11.11.2021.
//

#include "Spinner.h"
#include <pf_imgui/details/Spinner.h>
#include <string>

namespace pf::ui::ig {

Spinner::Spinner(Spinner::Config &&config) : Spinner(config.name, config.radius, config.thickness) {}

Spinner::Spinner(std::string_view elementName, float spinnerRadius, int spinnerThickness)
    : ItemElement(elementName), radius(spinnerRadius), thickness(spinnerThickness) {}

void Spinner::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  ImGui::Spinner(getName().c_str(), radius, thickness);
}

}  // namespace pf::ui::ig
