//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Collapsible.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>
#include <vector>

namespace pf::ui::ig {

// TODO: collapse support in children
// TODO: flags getter for children
class PF_IMGUI_EXPORT Layout : public Element, public Collapsible {
 public:
  explicit Layout(const std::string &elementName, AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No);
  Layout(const std::string &elementName, ShowBorder showBorder);

  [[nodiscard]] bool isDrawBorder() const;
  void setDrawBorder(bool drawBorder);

  [[nodiscard]] bool isScrollable() const;
  void setScrollable(bool scrollable);

  [[nodiscard]] virtual std::vector<Renderable*> getRenderables() = 0;



 protected:
  [[nodiscard]] bool renderCollapseButton();

 private:
  bool drawBorder;
  bool scrollable = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
