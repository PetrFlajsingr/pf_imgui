//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Layout : public Element {
 public:
  Layout(const std::string &elementName, bool showBorder);

  [[nodiscard]] bool isDrawBorder() const;
  void setDrawBorder(bool drawBorder);

  [[nodiscard]] bool isScrollable() const;
  void setScrollable(bool scrollable);

 private:
  bool drawBorder;
  bool scrollable = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
