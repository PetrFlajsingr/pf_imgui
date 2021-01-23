//
// Created by petr on 10/31/20.
//

#include "Container.h"
#include <utility>

namespace pf::ui::ig {

Container::Container(Container &&other) noexcept
    : Element(std::move(other)), children(std::move(other.children)), childrenInOrder(std::move(other.childrenInOrder)),
      childrenToRemove(std::move(other.childrenToRemove)) {}

Container &Container::operator=(Container &&other) noexcept {
  children = std::move(other.children);
  childrenInOrder = std::move(other.childrenInOrder);
  childrenToRemove = std::move(other.childrenToRemove);
  Element::operator=(std::move(other));
  return *this;
}

Container::Container(const std::string &elementName) : Element(elementName) {}

void Container::addChild(std::unique_ptr<Element> child) {
  childrenInOrder.emplace_back(*child);
  children[child->getName()] = std::move(child);
}

void Container::removeChild(const std::string &name) {
  if (const auto iter = children.find(name); iter != children.end()) {
    auto ptr = iter->second.get();
    auto ptrIter = std::ranges::find_if(childrenInOrder, [ptr](auto &child) { return &child.get() == ptr; });
    childrenInOrder.erase(ptrIter);
    children.erase(iter);
  }
}
void Container::enqueueChildRemoval(const std::string &name) { childrenToRemove.emplace_back(name); }

void Container::clear() {
  childrenInOrder.clear();
  children.clear();
  childrenToRemove.clear();
}

}// namespace pf::ui::ig