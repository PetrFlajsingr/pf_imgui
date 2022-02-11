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
    : ResizableLayout(std::string{config.name}, config.size, config.allowCollapse, config.showBorder,
                      config.persistent) {}

StackedLayout::Stack::Stack(StackedLayout &parent) : parent(parent) {}

void StackedLayout::Stack::setActive() { parent.setStackActive(*this); }

StackedLayout::StackedLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                             ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent) {}

StackedLayout::StackedLayout(const std::string &elementName, const Size &size, ShowBorder showBorder,
                             Persistent persistent)
    : StackedLayout(elementName, size, AllowCollapse::No, showBorder, persistent) {}

StackedLayout::StackedLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
                             Persistent persistent)
    : StackedLayout(elementName, size, allowCollapse, ShowBorder::No, persistent) {}

void StackedLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      if (selectedIndex.has_value()) {
        auto &activeStack = stacks[*selectedIndex];
        std::ranges::for_each(activeStack->getChildren(), [](auto &child) { child.render(); });
      }
    }
  }
}

StackedLayout::Stack &StackedLayout::pushStack() { return *stacks.emplace_back(std::make_unique<Stack>(*this)); }

void StackedLayout::popStack() {
  if (!stacks.empty()) { stacks.erase(stacks.end() - 1); }
}

void StackedLayout::removeStack(std::size_t index) {
#ifndef _MSC_VER  // TODO: MSVC internal compiler error
  if (index >= stacks.size()) { throw InvalidArgumentException("Index out of bounds: {}", index); }
#endif
  stacks.erase(stacks.begin() + static_cast<long long>(index));
}
void StackedLayout::moveStack(std::size_t srcIndex, std::size_t dstIndex) {
  auto stack = std::move(stacks[srcIndex]);
  stacks.erase(stacks.begin() + static_cast<long long>(srcIndex));
  stacks.insert(stacks.begin() + static_cast<long long>(dstIndex), std::move(stack));
}

std::size_t StackedLayout::getCurrentIndex() const { return *selectedIndex; }
void StackedLayout::setIndex(std::size_t index) {
#ifndef _MSC_VER  // TODO: MSVC internal compiler error
  if (index >= stacks.size()) { throw InvalidArgumentException("Index out of bounds: {}", index); }
#endif
  selectedIndex = index;
}
StackedLayout::Stack &StackedLayout::getCurrentStack() { return *stacks[*selectedIndex]; }

StackedLayout::Stack &StackedLayout::getStackAtIndex(std::size_t index) { return *stacks[index]; }

std::vector<Renderable *> StackedLayout::getRenderables() {
  return stacks | ranges::views::transform([](auto &stack) { return stack->getChildren() | ranges::views::all; })
      | ranges::views::join | ranges::views::transform([](auto &child) -> Renderable * { return &child; })
      | ranges::to_vector;
}

std::size_t StackedLayout::size() const { return stacks.size(); }

void StackedLayout::setStackActive(StackedLayout::Stack &stack) {
  for (const auto &[id, s] : ranges::views::enumerate(stacks)) {
    if (s.get() == &stack) {
      setIndex(id);
      break;
    }
  }
}

}  // namespace pf::ui::ig
