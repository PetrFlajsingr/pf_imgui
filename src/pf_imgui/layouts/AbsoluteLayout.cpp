//
// Created by xflajs00 on 25.01.2021.
//

#include "AbsoluteLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

AbsoluteLayout::AbsoluteLayout(AbsoluteLayout::Config &&config)
    : AbsoluteLayout(config.name, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

AbsoluteLayout::AbsoluteLayout(std::string_view elementName, Size initialSize, ShowBorder showBorder)
    : Layout(elementName, initialSize, showBorder) {}

void AbsoluteLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_HorizontalScrollbar : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  ScopeExit end{&ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    [[maybe_unused]] auto scrollApplier = applyScroll();
    std::ranges::for_each(children, [](auto &childPair) {
      const auto &[child, position] = childPair;
      ImGui::SetCursorPos(static_cast<ImVec2>(position));
      child->render();
    });
  }
}
void AbsoluteLayout::setChildPosition(const std::string &childName, Position position) {
  auto childrenAddr = children | ranges::views::addressof;
  if (const auto iter = std::ranges::find_if(childrenAddr, [childName](auto child) { return child->first->getName() == childName; });
      iter != childrenAddr.end()) {
    (*iter)->second = position;
  }
}
void AbsoluteLayout::removeChild(const std::string &childName) {
  if (auto iter = std::ranges::find_if(children, [childName](const auto &child) { return child.first->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}
std::vector<Renderable *> AbsoluteLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.first.get(); }) | ranges::to_vector;
}

}  // namespace pf::ui::ig
