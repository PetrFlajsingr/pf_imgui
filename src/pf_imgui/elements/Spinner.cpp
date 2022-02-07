//
// Created by xflajs00 on 11.11.2021.
//

#include "Spinner.h"
#include <pf_imgui/details/Spinner.h>
#include <string>

namespace pf::ui::ig {

Spinner::Spinner(const std::string &elementName, float radius, int thickness)
    : ItemElement(elementName), radius(radius), thickness(thickness) {}

void Spinner::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  ImGui::Spinner(getName().c_str(), radius, thickness);
}

}  // namespace pf::ui::ig
