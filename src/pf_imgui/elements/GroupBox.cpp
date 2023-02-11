//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

GroupBox::GroupBox(GroupBox::Config &&config) : GroupBox(config.name, config.label, config.size) {}

GroupBox::GroupBox(std::string_view elementName, std::string_view labelValue, Size s)
    : ElementWithID(elementName), label(std::string{labelValue}), size(s) {}

void GroupBox::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::BeginGroupPanel(label->get().c_str(), static_cast<ImVec2>(*size));
  ScopeExit end{&ImGui::EndGroupPanel};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
