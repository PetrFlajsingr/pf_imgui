//
// Created by petr on 10/31/20.
//

#include "Container.h"

namespace pf::ui::ig {

Container::Container(const std::string &elementName) : Element(elementName) {}

void Container::addChild(std::unique_ptr<Element> child) {
  childrenInOrder.emplace_back(*child);
  children[child->getName()] = std::move(child);
}
void Container::removeChild(const std::string &name) {
  if (const auto iter = children.find(name); iter != children.end()) {
    auto ptr = iter->second.get();
    auto ptrIter =
        std::ranges::find_if(childrenInOrder, [ptr](auto &child) { return &child.get() == ptr; });
    childrenInOrder.erase(ptrIter);
    children.erase(iter);
  }
}
const std::vector<std::reference_wrapper<Element>> &Container::getChildren() {
  std::ranges::for_each(childrenToRemove, [this](const auto &name) { removeChild(name); });
  childrenToRemove.clear();
  return childrenInOrder;
}
void Container::enqueueChildRemoval(const std::string &name) {
  childrenToRemove.emplace_back(name);
}
void Container::clear() {
  childrenInOrder.clear();
  children.clear();
  childrenToRemove.clear();
}
}// namespace pf::ui::ig