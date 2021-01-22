//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <pf_common/RAII.h>
#include <utility>

namespace pf::ui::ig {

Element::Element(std::string elementName) : name(std::move(elementName)) {}

Element::Element(Element &&other) noexcept {
  if (isMoved) { return; }
  name = std::move(other.name);
  visibility = other.visibility;
  isMoved = true;
}

Element &Element::operator=(Element &&other) noexcept {
  if (isMoved) { return *this; }
  name = std::move(other.name);
  visibility = other.visibility;
  isMoved = true;
  return *this;
}

const std::string &Element::getName() const { return name; }

Visibility Element::getVisibility() const { return visibility; }

void Element::setVisibility(Visibility visi) { visibility = visi; }

void Element::render() {
  if (visibility == Visibility::Visible) {
    if (enabled == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    auto raiiEnabled = pf::RAII([this]() {
      if (enabled == Enabled::No) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }
    });
    renderImpl();
  }
}

void Element::setEnabled(Enabled eleState) { enabled = eleState; }

Enabled Element::getEnabled() const { return enabled; }

}// namespace pf::ui::ig