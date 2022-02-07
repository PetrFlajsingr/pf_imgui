//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

GroupBox::GroupBox(const std::string &name, std::unique_ptr<Resource<std::string>> label, Size s)
    : Element(name), Labellable(std::move(label)), Resizable(s) {}

void GroupBox::renderImpl() {
  ImGui::BeginGroupPanel(getLabel().get().c_str(), getSize().asImVec());
  pf::RAII end{[] { ImGui::EndGroupPanel(); }};
  std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
}

}  // namespace pf::ui::ig
