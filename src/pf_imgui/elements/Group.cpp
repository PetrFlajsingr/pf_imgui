//
// Created by petr on 10/31/20.
//

#include "Group.h"
#include <imgui.h>

namespace pf::ui::ig {

Group::Group(const std::string &elementName, const std::string &caption)
    : Element(elementName), Container(elementName), LabeledElement(elementName, caption) {}

void Group::renderImpl() {
  ImGui::BeginGroup();
  ImGui::Text("%s:", getLabel().c_str());
  ImGui::Separator();
  std::ranges::for_each(getChildren(), [](auto &child) { child.get().render(); });
  ImGui::Separator();
  ImGui::EndGroup();
}

}// namespace pf::ui