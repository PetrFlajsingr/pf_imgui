//
// Created by petr on 1/23/21.
//

#include "Renderable.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Renderable::Renderable(std::string name) : name(std::move(name)) {}

Renderable::Renderable(Renderable &&other) noexcept : visibility(other.visibility) {}

Renderable &Renderable::operator=(Renderable &&other) noexcept {
  visibility = other.visibility;
  return *this;
}

Visibility Renderable::getVisibility() const { return visibility; }

void Renderable::setVisibility(Visibility visi) { visibility = visi; }

void Renderable::setEnabled(Enabled eleState) { enabled = eleState; }

Enabled Renderable::getEnabled() const { return enabled; }

void Renderable::render() {
  if (visibility == Visibility::Visible) {
    if (enabled == Enabled::No) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
  }
}
const std::string &Renderable::getName() const { return name; }

}  // namespace pf::ui::ig
