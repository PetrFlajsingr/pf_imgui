//
// Created by xflajs00 on 17.11.2021.
//

#include "Expander.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

Expander::Expander(const std::string &name, const std::string &label, Size s)
    : Element(name), Labellable(label), Resizable(s) {}

void Expander::renderImpl() {
  ImGui::BeginGroupPanel(getLabel().c_str(), getSize().asImVec());
  pf::RAII end{[] { ImGui::EndGroupPanel(); }};
  std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
}

}  // namespace pf::ui::ig
