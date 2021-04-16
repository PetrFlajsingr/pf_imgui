//
// Created by petr on 10/31/20.
//

#include "ElementContainer.h"
#include <utility>

namespace pf::ui::ig {

ElementContainer::ElementContainer(ElementContainer &&other) noexcept
    : children(std::move(other.children)), childrenInOrder(std::move(other.childrenInOrder)),
      childrenToRemove(std::move(other.childrenToRemove)) {}

ElementContainer &ElementContainer::operator=(ElementContainer &&other) noexcept {
  children = std::move(other.children);
  childrenInOrder = std::move(other.childrenInOrder);
  childrenToRemove = std::move(other.childrenToRemove);
  return *this;
}

void ElementContainer::addChild(std::unique_ptr<Element> child) {
  childrenInOrder.emplace_back(*child);
  children[child->getName()] = std::move(child);
}

void ElementContainer::insertChild(std::unique_ptr<Element> child, std::size_t index) {
  if (index > childrenInOrder.size()) { throw InvalidArgumentException("Index out of bounds: {}", index); }
  childrenInOrder.insert(childrenInOrder.begin() + index, *child);
  children[child->getName()] = std::move(child);
}

void ElementContainer::removeChild(const std::string &name) {
  if (const auto iter = children.find(name); iter != children.end()) {
    auto ptr = iter->second.get();
    auto ptrIter = std::ranges::find_if(childrenInOrder, [ptr](auto &child) { return &child.get() == ptr; });
    childrenInOrder.erase(ptrIter);
    children.erase(iter);
  }
}

void ElementContainer::enqueueChildRemoval(const std::string &name) { childrenToRemove.emplace_back(name); }

void ElementContainer::clear() {
  childrenInOrder.clear();
  children.clear();
  childrenToRemove.clear();
}

}// namespace pf::ui::ig