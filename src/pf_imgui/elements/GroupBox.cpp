//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>

namespace pf::ui::ig {

GroupBox::GroupBox(const std::string &name, const std::string &label, const Size &s)
    : Element(name), Labellable(label), Resizable(s) {}

void GroupBox::renderImpl() {
  ImGui::BeginGroupPanel(getLabel().c_str(), getSize().asImVec());
  pf::RAII end{[] { ImGui::EndGroupPanel(); }};
  std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
}

}// namespace pf::ui::ig