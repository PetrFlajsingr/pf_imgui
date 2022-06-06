//
// Created by petr on 6/13/21.
//

#include "AnchorLayout.h"
#include <algorithm>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

AnchorLayout::AnchorLayout(AnchorLayout::Config &&config)
    : Layout(std::string{config.name}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

AnchorLayout::AnchorLayout(const std::string &elementName, const Size &size, ShowBorder showBorder)
    : Layout(elementName, size, showBorder) {}

std::vector<Renderable *> AnchorLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.element.get(); })
      | ranges::to_vector;
}

void AnchorLayout::setChildPosition(const std::string &childName, Position position) {
  if (auto child = findIf(children | ranges::views::addressof,
                          [childName](auto child) { return child->element->getName() == childName; });
      child.has_value()) {
    (*child)->positionable->setPosition(position);
  }
}

void AnchorLayout::removeChild(const std::string &childName) {
  if (auto iter = std::ranges::find_if(
          children, [childName](const auto &child) { return child.element->getName() == childName; });
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
      positionable->setPosition(positionable->getPosition().moveDelta(static_cast<float>(deltaWidth), 0));
    }
    if (anchorFlags.is(Anchor::Top)) {
      if (anchorFlags.is(Anchor::Bottom)) { addHeight(deltaHeight); }
    } else if (anchorFlags.is(Anchor::Bottom)) {
      positionable->setPosition(positionable->getPosition().moveDelta(0, static_cast<float>(deltaHeight)));
    }
  });
  Resizable::setSize(s);
}

void AnchorLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    std::ranges::for_each(children, [](auto &childData) {
      auto &[child, positionable, anchor, _1, _2] = childData;
      ImGui::SetCursorPos(static_cast<ImVec2>(positionable->getPosition()));
      child->render();
    });
  }
}

}  // namespace pf::ui::ig
