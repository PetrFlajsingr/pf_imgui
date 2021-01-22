//
// Created by petr on 10/31/20.
//

#include "Element.h"
#include <imgui.h>
#include <imgui_internal.h>
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
    if (state == State::Disabled) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    renderImpl();
    if (state == State::Disabled) {
      ImGui::PopItemFlag();
      ImGui::PopStyleVar();
    }
  }
}

void Element::setState(State eleState) { state = eleState; }

State Element::getState() const { return state; }

}// namespace pf::ui::ig