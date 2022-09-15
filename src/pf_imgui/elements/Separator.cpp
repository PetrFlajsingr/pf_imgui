//
// Created by petr on 5/15/21.
//

#include "Separator.h"
#include <string>

namespace pf::ui::ig {

Separator::Separator(Separator::Config &&config) : Separator(config.name) {}

Separator::Separator(std::string_view elementName) : Element(elementName) {}

void Separator::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  ImGui::Separator();
}

}  // namespace pf::ui::ig
