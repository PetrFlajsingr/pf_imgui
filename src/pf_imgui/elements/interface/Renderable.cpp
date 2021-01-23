//
// Created by petr on 1/23/21.
//

#include "Renderable.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

Renderable::Renderable(Renderable &&other) noexcept { visibility = other.visibility; }

Renderable &Renderable::operator=(Renderable &&other) noexcept {
  visibility = other.visibility;
  ;
  return *this;
}

Visibility Renderable::getVisibility() const { return visibility; }

void Renderable::setVisibility(Visibility visi) { visibility = visi; }

void Renderable::setEnabled(Enabled eleState) { enabled = eleState; }

Enabled Renderable::getEnabled() const { return enabled; }

void Renderable::render() {
  if (visibility == Visibility::Visible) {
    if (enabled == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      auto raiiEnabled = pf::RAII([] {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      });
      renderImpl();
    } else {
      renderImpl();
    }
  }
}
}// namespace pf::ui::ig