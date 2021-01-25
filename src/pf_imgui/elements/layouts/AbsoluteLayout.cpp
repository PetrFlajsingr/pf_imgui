//
// Created by xflajs00 on 25.01.2021.
//

#include "AbsoluteLayout.h"

namespace pf::ui::ig {

AbsoluteLayout::AbsoluteLayout(const std::string &elementName, const ImVec2 &size, bool showBorder)
    : Layout(elementName, size, showBorder) {}

void AbsoluteLayout::renderImpl() {
  ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder());
  auto posChildren = childrenWithPos();
  std::ranges::for_each(posChildren, [](auto &&childPair) {
    auto [child, positionable] = childPair;
    ImGui::SetCursorPos(positionable->getPosition());
    child->render();
  });

  ImGui::EndChild();
}
void AbsoluteLayout::setChildPosition(const std::string &name, ImVec2 position) {
  if (auto child = findIf(getChildren() | ranges::views::addressof,
                          [name](const auto &child) { return child->getName() == name; });
      child.has_value()) {
    reinterpret_cast<Positionable *>(child.value())->setPosition(position);
  } else {
    throw StackTraceException::fmt("Child not found: {}", name);
  }
}
}// namespace pf::ui::ig