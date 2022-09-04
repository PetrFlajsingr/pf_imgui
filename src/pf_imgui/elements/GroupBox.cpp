//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

GroupBox::GroupBox(GroupBox::Config &&config)
    : ElementWithID(std::string{config.name.value}), size(config.size), label(std::string{config.label.value}) {}

GroupBox::GroupBox(const std::string &elementName, const std::string &labelValue, Size s)
    : ElementWithID(elementName), size(s), label(labelValue) {}

void GroupBox::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::BeginGroupPanel(label->get().c_str(), static_cast<ImVec2>(*size));
  RAII end{ImGui::EndGroupPanel};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
