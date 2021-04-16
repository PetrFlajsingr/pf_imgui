//
// Created by petr on 1/25/21.
//

#include "StretchLayout.h"
#include "AbsoluteLayout.h"

namespace pf::ui::ig {

StretchLayout::StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch,
                             AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), stretch(stretch) {}

StretchLayout::StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch, ShowBorder showBorder,
                             Persistent persistent)
    : StretchLayout(elementName, size, stretch, AllowCollapse::No, showBorder, persistent) {}

StretchLayout::StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch,
                             AllowCollapse allowCollapse, Persistent persistent)
    : StretchLayout(elementName, size, stretch, allowCollapse, ShowBorder::No, persistent) {}

Stretch StretchLayout::getStretch() const { return stretch; }
void StretchLayout::setStretch(Stretch newStretch) {
  stretch = newStretch;
  switch (stretch) {
    case Stretch::Width: setSize(ImVec2{0, getSize().y}); break;
    case Stretch::Height: setSize(ImVec2{getSize().x, 0}); break;
    case Stretch::All: setSize(ImVec2{0, 0}); break;
  }
}
Element &StretchLayout::getChild() {
  if (child == nullptr) {
    throw StackTraceException("Child not present");
  }
  return *dynamic_cast<Element *>(child.get());
}
void StretchLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      child->setSize(getSize());
      renderableChild->render();
    }
  }
  ImGui::EndChild();
}
std::vector<Renderable *> StretchLayout::getRenderables() { return {dynamic_cast<Renderable *>(child.get())}; }
bool StretchLayout::hasChild() const {
  return child != nullptr;
}

}// namespace pf::ui::ig
