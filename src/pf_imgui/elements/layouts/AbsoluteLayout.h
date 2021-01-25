//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H

#include "BoxLayout.h"
#include "ResizableLayout.h"
#include "layout_decorators.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Positionable.h>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT AbsoluteLayout : public ResizableLayout {
 public:
  AbsoluteLayout(const std::string &elementName, const ImVec2 &size, bool showBorder = false);

  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child.first; });
  }

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::constructible_from<T, std::string, Args...> auto &
  createChild(std::string name, ImVec2 position, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw StackTraceException::fmt("{} already present in ui", name);
    }
    constexpr auto IsPositionable = std::derived_from<T, Positionable>;
    using CreateType = std::conditional_t<IsPositionable, T, PositionDecorator<T>>;
    auto child = std::make_unique<CreateType>(position, name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    children.template emplace_back(std::move(child), dynamic_cast<Positionable *>(ptr));
    return *ptr;
  }

  void setChildPosition(const std::string &name, ImVec2 position);

  void removeChild(const std::string &name);

 protected:
  void renderImpl() override;

 private:
  std::vector<std::pair<std::unique_ptr<Element>, Positionable *>> children;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
