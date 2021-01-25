//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H

#include "BoxLayout.h"
#include "Layout.h"
#include "layout_decorators.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Positionable.h>
#include <range/v3/view/transform.hpp>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT AbsoluteLayout : public Layout {
 public:
  AbsoluteLayout(const std::string &elementName, const ImVec2 &size, bool showBorder = false);

  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child; });
  }

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...> PositionDecorator<T> &
  createChild(std::string name, ImVec2 position, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw StackTraceException::fmt("{} already present in ui", name);
    }
    auto child = std::make_unique<PositionDecorator<T>>(position, name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    pushChild(std::move(child));
    return *ptr;
  }

  void setChildPosition(const std::string &name, ImVec2 position);

 protected:
  void renderImpl() override;

 private:
  inline auto childrenWithPos() {
    return children | ranges::views::transform([](auto &child) {
             return std::make_pair(child.get(), reinterpret_cast<Positionable *>(child.get()));
           });
  }
  std::vector<std::unique_ptr<Element>> children;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
