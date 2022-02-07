//
// Created by petr on 5/15/21.
//

#include "Separator.h"
#include <string>

namespace pf::ui::ig {

Separator::Separator(const std::string &name) : Element(name) {}

void Separator::renderImpl() {
  auto colorStyle = setColorStack();
  ImGui::Separator();
}

}  // namespace pf::ui::ig
