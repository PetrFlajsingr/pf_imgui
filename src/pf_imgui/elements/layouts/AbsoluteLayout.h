//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H

#include "BoxLayout.h"
#include "Layout.h"
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
