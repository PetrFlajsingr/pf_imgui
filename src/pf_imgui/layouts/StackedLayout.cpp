//
// Created by petr on 1/24/21.
//

#include "StackedLayout.h"
#include <memory>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/join.hpp>
#include <string>
#include <utility>

namespace pf::ui::ig {

StackedLayout::StackedLayout(StackedLayout::Config &&config)
    : Layout(std::string{config.name.value}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

StackedLayout::Stack::Stack(StackedLayout &owner) : parent(owner) {}

void StackedLayout::Stack::setActive() { parent.setStackActive(*this); }

StackedLayout::StackedLayout(const std::string &elementName, const Size &initialSize, ShowBorder showBorder)
    : Layout(elementName, initialSize, showBorder) {}

void StackedLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    if (selectedIndex.has_value()) {
      auto &activeStack = stacks[*selectedIndex];
      std::ranges::for_each(activeStack->getChildren(), [](auto &child) { child.render(); });
    }
  }
}

StackedLayout::Stack &StackedLayout::pushStack() { return *stacks.emplace_back(std::make_unique<Stack>(*this)); }

void StackedLayout::popStack() {
  if (!stacks.empty()) { stacks.erase(stacks.end() - 1); }
}

void StackedLayout::removeStack(std::size_t index) { stacks.erase(stacks.begin() + static_cast<long long>(index)); }

void StackedLayout::moveStack(std::size_t srcIndex, std::size_t dstIndex) {
  auto stack = std::move(stacks[srcIndex]);
  stacks.erase(stacks.begin() + static_cast<long long>(srcIndex));
  stacks.insert(stacks.begin() + static_cast<long long>(dstIndex), std::move(stack));
}

std::size_t StackedLayout::getCurrentIndex() const { return *selectedIndex; }

void StackedLayout::setIndex(std::size_t index) { selectedIndex = index; }

StackedLayout::Stack &StackedLayout::getCurrentStack() { return *stacks[*selectedIndex]; }

StackedLayout::Stack &StackedLayout::getStackAtIndex(std::size_t index) { return *stacks[index]; }

std::vector<Renderable *> StackedLayout::getRenderables() {
  return stacks | ranges::views::transform([](auto &stack) { return stack->getChildren() | ranges::views::all; })
      | ranges::views::join | ranges::views::transform([](auto &child) -> Renderable * { return &child; })
      | ranges::to_vector;
}

std::size_t StackedLayout::stackSize() const { return stacks.size(); }

void StackedLayout::setStackActive(StackedLayout::Stack &stack) {
  for (const auto &[stackId, s] : ranges::views::enumerate(stacks)) {
    if (s.get() == &stack) {
      setIndex(stackId);
      break;
    }
  }
}

}  // namespace pf::ui::ig
