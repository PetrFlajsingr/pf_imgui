//
// Created by petr on 1/24/21.
//

#include "StackedLayout.h"
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/join.hpp>
#include <utility>

namespace pf::ui::ig {

StackedLayout::StackedLayout(const std::string &elementName, const ImVec2 &size, bool showBorder)
    : ResizableLayout(elementName, size, showBorder) {}

void StackedLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  if (ImGui::BeginChild(getName().c_str(), getSize(), isDrawBorder(), flags)) {
    if (selectedIndex.has_value()) {
      auto &activeStack = stacks[*selectedIndex];
      std::ranges::for_each(activeStack.getChildren(), [](auto &child) { child.render(); });
    }
  }
  ImGui::EndChild();
}

StackedLayout::StackContainer &StackedLayout::pushStack() {
  stacks.emplace_back();
  return stacks.back();
}

void StackedLayout::removeStack(std::size_t index) { stacks.erase(stacks.begin() + index); }

void StackedLayout::moveStack(std::size_t srcIndex, std::size_t dstIndex) {
  auto stack = std::move(stacks[srcIndex]);
  stacks.erase(stacks.begin() + srcIndex);
  stacks.insert(stacks.begin() + dstIndex, std::move(stack));
}

std::size_t StackedLayout::getCurrentIndex() const { return *selectedIndex; }

void StackedLayout::setIndex(std::size_t index) { selectedIndex = index; }

StackedLayout::StackContainer &StackedLayout::getCurrentStack() { return stacks[*selectedIndex]; }
StackedLayout::StackContainer &StackedLayout::getStackAtIndex(std::size_t index) { return stacks[index]; }

std::vector<Renderable *> StackedLayout::getRenderables() {
  return stacks | ranges::views::transform([](auto &stack) { return stack.getChildren() | ranges::views::all; })
      | ranges::views::join | ranges::views::transform([](auto &child) -> Renderable * { return &child; })
      | ranges::to_vector;
}

}// namespace pf::ui::ig