//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

GroupBox::GroupBox(const std::string &name, const std::string &label, Size s)
    : Element(name), Labellable(label), Resizable(s) {}

void GroupBox::renderImpl() {
  ImGui::BeginGroupPanel(getLabel().c_str(), getSize().asImVec());
  RAII end{ImGui::EndGroupPanel};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
