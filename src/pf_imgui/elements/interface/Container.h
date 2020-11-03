//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUICONTAINER_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUICONTAINER_H

#include <pf_common/exceptions/StackTraceException.h>
#include "Element.h"
#include <map>
#include <memory>
#include <string>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Container : public virtual Element {
 public:
  explicit Container(const std::string &elementName);
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...>
      std::shared_ptr<T> createChild(std::string name, Args &&... args) {
    auto child = std::make_shared<T>(name, std::forward<Args>(args)...);
    addChild(child);
    return child;
  }
  void addChild(std::shared_ptr<Element> child);
  void removeChild(const std::string &name);

  void enqueueChildRemoval(const std::string &name);

  template<std::derived_from<Element> T>
  [[nodiscard]] std::shared_ptr<T> childByName(const std::string &name) {
    if (const auto iter = children.find(name); iter != children.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(iter->second); result != nullptr) {
        return result;
      }
      throw StackTraceException::fmt("Wrong type for child: '{}' in '{}'", name, getName());
    }
    throw StackTraceException::fmt("Child not found: '{}' in '{}'", name, getName());
  }

  [[nodiscard]] const std::vector<std::reference_wrapper<Element>> &getChildren();

  void clear();

 private:
  std::map<std::string, std::shared_ptr<Element>> children;
  std::vector<std::reference_wrapper<Element>> childrenInOrder;
  std::vector<std::string> childrenToRemove;
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUICONTAINER_H
