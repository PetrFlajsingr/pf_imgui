//
// Created by petr on 1/23/21.
//

#include "Renderable.h"
#include <imgui.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Renderable::Renderable(std::string renderableName) : name(std::move(renderableName)) {}

Renderable::Renderable(Renderable &&other) noexcept : visibility(std::move(other.visibility)) {}

Renderable &Renderable::operator=(Renderable &&other) noexcept {
  visibility = std::move(other.visibility);
  enabled = std::move(other.enabled);
  return *this;
}

void Renderable::render() {
  if (*visibility == Visibility::Visible) {
    if (!*enabled) {
      ImGui::BeginDisabled();
      RAII raiiEnabled{ImGui::EndDisabled};
      renderImpl();
    } else {
      renderImpl();
    }
  }
}

const std::string &Renderable::getName() const { return name; }

void Renderable::setName(std::string newName) { name = std::move(newName); }

}  // namespace pf::ui::ig
