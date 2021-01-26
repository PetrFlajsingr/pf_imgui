//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>
#include <vector>

namespace pf::ui::ig {

// TODO: collapse support in children
class PF_IMGUI_EXPORT Layout : public Element {
 public:
  Layout(const std::string &elementName, bool showBorder);

  [[nodiscard]] bool isDrawBorder() const;
  void setDrawBorder(bool drawBorder);

  [[nodiscard]] bool isScrollable() const;
  void setScrollable(bool scrollable);

  [[nodiscard]] bool isCollapsible() const;
  void setCollapsible(bool collapsible);

  [[nodiscard]] virtual std::vector<Renderable*> getRenderables() = 0;

 private:
  bool drawBorder;
  bool scrollable = false;
  bool collapsible = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
