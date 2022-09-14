//
// Created by petr on 6/13/21.
//

#include "AnchorLayout.h"
#include <algorithm>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

AnchorLayout::AnchorLayout(AnchorLayout::Config &&config)
    : Layout(config.name.value, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {
  size.addListener([this, previousSize = *size](Size newSize) mutable {
    onSizeUpdated(previousSize);
    previousSize = newSize;
  });
}

AnchorLayout::AnchorLayout(std::string_view elementName, const Size &initialSize, ShowBorder showBorder)
    : Layout(elementName, initialSize, showBorder) {
  size.addListener([this, previousSize = *size](Size newSize) mutable {
    onSizeUpdated(previousSize);
    previousSize = newSize;
  });
}

std::vector<Renderable *> AnchorLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.element.get(); })
      | ranges::to_vector;
}

void AnchorLayout::setChildPosition(const std::string &childName, Position position) {
  if (auto foundChild = findIf(children | ranges::views::addressof,
                               [childName](auto child) { return child->element->getName() == childName; });
      foundChild.has_value()) {
    (*foundChild)->position = position;
  }
}

void AnchorLayout::removeChild(const std::string &childName) {
  if (auto iter = std::ranges::find_if(
          children, [childName](const auto &child) { return child.element->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}

void AnchorLayout::onSizeUpdated(Size previousSize) {
  using namespace pf::enum_operators;
  if (static_cast<float>(previousSize.width) <= 0 || static_cast<float>(previousSize.height) <= 0) { return; }
  const auto deltaWidth = size->width - previousSize.width;
  const auto deltaHeight = size->height - previousSize.height;
  std::ranges::for_each(children, [&](auto &childData) {
    auto &[child, position, anchor, addWidth, addHeight] = childData;
    const auto anchorFlags = Flags{anchor};
    if (anchorFlags.is(Anchor::Left)) {
      if (anchorFlags.is(Anchor::Right)) { addWidth(deltaWidth); }
    } else if (anchorFlags.is(Anchor::Right)) {
      position = position.moveDelta(static_cast<float>(deltaWidth), 0);
    }
    if (anchorFlags.is(Anchor::Top)) {
      if (anchorFlags.is(Anchor::Bottom)) { addHeight(deltaHeight); }
    } else if (anchorFlags.is(Anchor::Bottom)) {
      position = position.moveDelta(0, static_cast<float>(deltaHeight));
    }
  });
}

void AnchorLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags = isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar
                                    : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    std::ranges::for_each(children, [](auto &childData) {
      const auto &[child, position, anchor, _1, _2] = childData;
      ImGui::SetCursorPos(static_cast<ImVec2>(position));
      child->render();
    });
  }
}

}  // namespace pf::ui::ig
