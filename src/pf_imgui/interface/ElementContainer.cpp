//
// Created by petr on 10/31/20.
//

#include "ElementContainer.h"
#include <range/v3/range/conversion.hpp>
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
  childrenInOrder.insert(childrenInOrder.begin() + static_cast<long long>(index), *child);
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
std::vector<Renderable *> ElementContainer::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.second.get(); })
      | ranges::to_vector;
}

}  // namespace pf::ui::ig
