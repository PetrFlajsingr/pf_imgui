//
// Created by petr on 1/24/21.
//

#include "StackedLayout.h"
#include <utility>

namespace pf::ui::ig {

void StackedLayout::renderImpl() {
  ImGui::BeginChild(getName().c_str(), getSize(), false);
  if (selectedIndex.has_value()) {
    auto &activeStack = stacks[*selectedIndex];
    std::ranges::for_each(activeStack.getChildren(), [](auto &child) { child.render(); });
  }
  ImGui::EndChild();
}

void StackedLayout::pushStack() { stacks.emplace_back(); }

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

}// namespace pf::ui::ig