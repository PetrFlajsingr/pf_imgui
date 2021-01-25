//
// Created by petr on 1/25/21.
//

#include "StretchLayout.h"
#include "AbsoluteLayout.h"

namespace pf::ui::ig {

StretchLayout::StretchLayout(const std::string &elementName, Stretch stretch, const ImVec2 &size, bool showBorder)
    : ResizableLayout(elementName, size, showBorder), stretch(stretch) {
  setStretch(stretch);
}

Stretch StretchLayout::getStretch() const { return stretch; }

void StretchLayout::setStretch(Stretch newStretch) {
  stretch = newStretch;
  switch (stretch) {
    case Stretch::Width: setSize(ImVec2{0, getSize().y}); break;
    case Stretch::Height: setSize(ImVec2{getSize().x, 0}); break;
    case Stretch::All: setSize(ImVec2{0, 0}); break;
  }
}

Element &StretchLayout::getChild() { return *dynamic_cast<Element *>(child.get()); }

void StretchLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder(), flags)) {}
  ImGui::EndChild();
}

}// namespace pf::ui::ig
