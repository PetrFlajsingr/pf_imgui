//
// Created by petr on 6/13/21.
//

#include "AnchorLayout.h"
#include <algorithm>
#include <imgui_internal.h>

namespace pf::ui::ig {

AnchorLayout::AnchorLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                           ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent) {}

AnchorLayout::AnchorLayout(const std::string &elementName, const Size &size, ShowBorder showBorder,
                           Persistent persistent)
    : AnchorLayout(elementName, size, AllowCollapse::No, showBorder, persistent) {}

AnchorLayout::AnchorLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                           Persistent persistent)
    : AnchorLayout(elementName, size, allowCollapse, ShowBorder::No, persistent) {}

std::vector<Renderable *> AnchorLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.element.get(); })
      | ranges::to_vector;
}
void AnchorLayout::setChildPosition(const std::string &name, ImVec2 position) {
  if (auto child =
          findIf(children | ranges::views::addressof, [name](auto child) { return child->element->getName() == name; });
      child.has_value()) {
    child.value()->positionable->setPosition(position);
  } else {
    throw IdNotFoundException("Child not found: {}", name);
  }
}
void AnchorLayout::removeChild(const std::string &name) {
  if (auto iter =
          std::ranges::find_if(children, [name](const auto &child) { return child.element->getName() == name; });
      iter != children.end()) {
    children.erase(iter);
  }
}
void AnchorLayout::setSize(const Size &s) {
  using namespace pf::enum_operators;
  const auto deltaWidth = s.width - getSize().width;
  const auto deltaHeight = s.height - getSize().height;
  std::ranges::for_each(children, [&](auto &childData) {
    auto &[child, positionable, anchor, addWidth, addHeight] = childData;
    const auto anchorFlags = Flags{anchor};
    if (anchorFlags.is(Anchor::Left)) {
      if (anchorFlags.is(Anchor::Right)) { addWidth(deltaWidth); }
    } else if (anchorFlags.is(Anchor::Right)) {
      positionable->setPosition(positionable->getPosition() + ImVec2{deltaWidth, 0});
    }
    if (anchorFlags.is(Anchor::Top)) {
      if (anchorFlags.is(Anchor::Bottom)) { addHeight(deltaHeight); }
    } else if (anchorFlags.is(Anchor::Bottom)) {
      positionable->setPosition(positionable->getPosition() + ImVec2{0, deltaHeight});
    }
  });
  Resizable::setSize(s);
}

void AnchorLayout::renderImpl() {
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      std::ranges::for_each(children, [](auto &childData) {
        auto &[child, positionable, anchor, _1, _2] = childData;
        ImGui::SetCursorPos(positionable->getPosition());
        child->render();
      });
    }
  }
  ImGui::EndChild();
}
}// namespace pf::ui::ig