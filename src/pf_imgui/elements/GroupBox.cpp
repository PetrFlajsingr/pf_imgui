//
// Created by xflajs00 on 17.11.2021.
//

#include "GroupBox.h"
#include <pf_imgui/details/GroupBox.h>
#include <string>

namespace pf::ui::ig {

GroupBox::GroupBox(GroupBox::Config &&config)
    : ElementWithID(std::string{config.name.value}), Labellable(std::string{config.label.value}),
      Resizable(config.size) {}

GroupBox::GroupBox(const std::string &name, const std::string &label, Size s)
    : ElementWithID(name), Labellable(label), Resizable(s) {}

void GroupBox::renderImpl() {
  [[maybe_unused]] auto scopedFont = applyFont();
  ImGui::BeginGroupPanel(getLabel().c_str(), static_cast<ImVec2>(getSize()));
  RAII end{ImGui::EndGroupPanel};
  std::ranges::for_each(getChildren(), &Renderable::render);
}

}  // namespace pf::ui::ig
