//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H

#include "Layout.h"
#include <memory>
#include <pf_common/algorithms.h>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Resizable.h>
#include <range/v3/view/addressof.hpp>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT BoxLayout : public Layout {
 public:
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size = ImVec2{0, 0},
            bool showBorder = false);

  [[nodiscard]] LayoutDirection getLayoutDirection() const;
  void setLayoutDirection(LayoutDirection layoutDirection);

  [[nodiscard]] bool isShowBorder() const;
  void setShowBorder(bool showBorder);

  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child; });
  }

  void pushChild(std::unique_ptr<Element> child) { children.emplace_back(std::move(child)); }

  void insertChild(std::unique_ptr<Element> child, std::size_t index) {
    if (index > children.size()) { throw StackTraceException::fmt("Index out of bounds: {}", index); }
    children.insert(children.begin() + index, std::move(child));
  }

  void removeChild(const std::string &name) {
    if (auto iter = std::ranges::find_if(children, [name](const auto &child) { return child->getName() == name; });
        iter != children.end()) {
      children.erase(iter);
    }
  }

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...> T &
  createChild(std::string name, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw StackTraceException::fmt("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    pushChild(std::move(child));
    return *ptr;
  }

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...> T &
  createChildAtIndex(std::size_t index, std::string name, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw StackTraceException::fmt("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

 protected:
  void renderImpl() override;

 private:
  LayoutDirection layoutDirection;
  bool showBorder;
  std::vector<std::unique_ptr<Element>> children;

  void renderTopToBottom();
  void renderLeftToRight();
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
