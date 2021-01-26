//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H

#include "ResizableLayout.h"
#include <memory>
#include <pf_common/algorithms.h>
#include <pf_common/enums.h>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Resizable.h>
#include <range/v3/view/addressof.hpp>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT BoxLayout : public ResizableLayout {
 public:
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const ImVec2 &size = ImVec2{0, 0},
            bool showBorder = false);

  [[nodiscard]] LayoutDirection getLayoutDirection() const;
  void setLayoutDirection(LayoutDirection layoutDirection);

  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child; });
  }

  std::vector<Renderable *> getRenderables() override;

  void pushChild(std::unique_ptr<Element> child);

  void insertChild(std::unique_ptr<Element> child, std::size_t index);

  void removeChild(const std::string &name);

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
  std::vector<std::unique_ptr<Element>> children;

  void renderTopToBottom();
  void renderLeftToRight();
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_BOXLAYOUT_H
