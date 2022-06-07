//
// Created by xflajs00 on 11.11.2021.
//

#include "Spinner.h"
#include <pf_imgui/details/Spinner.h>
#include <string>

namespace pf::ui::ig {

Spinner::Spinner(Spinner::Config &&config)
    : ItemElement(std::string{config.name.value}), radius(config.radius), thickness(config.thickness) {}

Spinner::Spinner(const std::string &elementName, float radius, int thickness)
    : ItemElement(elementName), radius(radius), thickness(thickness) {}

void Spinner::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  ImGui::Spinner(getName().c_str(), radius, thickness);
}

}  // namespace pf::ui::ig
