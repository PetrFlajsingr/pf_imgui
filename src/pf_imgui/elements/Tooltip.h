//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Tooltip : public Element, public ElementContainer {
 public:
  explicit Tooltip(const std::string &elementName);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H
