//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H

#include "Element.h"
#include <memory>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/_export.h>
#include <range/v3/view/all.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Container : public virtual Element {
 public:
  explicit Container(const std::string &elementName);

  Container(Container &&other) noexcept;
  Container &operator=(Container &&other) noexcept;

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...> T &
  createChild(std::string name, Args &&...args) {
    if (const auto iter = children.find(name); iter != children.end()) {
      throw StackTraceException::fmt("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    addChild(std::move(child));
    return *ptr;
  }
  void removeChild(const std::string &name);
  void addChild(std::unique_ptr<Element> child);

  void enqueueChildRemoval(const std::string &name);

  template<std::derived_from<Element> T>
  [[nodiscard]] T &childByName(const std::string &name) {
    if (const auto iter = children.find(name); iter != children.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(iter->second); result != nullptr) { return result; }
      throw StackTraceException::fmt("Wrong type for child: '{}' in '{}'", name, getName());
    }
    throw StackTraceException::fmt("Child not found: '{}' in '{}'", name, getName());
  }

  [[nodiscard]] inline auto getChildren() {
    return children | ranges::views::values
        | ranges::views::transform([](auto &childPtr) -> Element & { return *childPtr; }) | ranges::views::all;
  }

  void clear();

 private:
  std::unordered_map<std::string, std::unique_ptr<Element>> children;
  std::vector<std::reference_wrapper<Element>> childrenInOrder;
  std::vector<std::string> childrenToRemove;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H
