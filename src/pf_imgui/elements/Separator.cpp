//
// Created by petr on 5/15/21.
//

#include "Separator.h"
#include <pf_common/exceptions/StackTraceException.h>

pf::ui::ig::Separator::Separator(const std::string &name) : Element(name) {}

void pf::ui::ig::Separator::renderImpl() {
  ImGui::Separator();
}
