//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Container.h>
#include <pf_imgui/elements/interface/Element.h>
#include <pf_imgui/elements/interface/Resizable.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Layout : public Element, public Resizable {
 public:
  Layout(const std::string &elementName, const ImVec2 &size);

 private:
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_LAYOUT_H
