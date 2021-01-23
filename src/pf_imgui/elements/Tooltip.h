//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H

#include <pf_imgui/elements/interface/Container.h>

namespace pf::ui::ig {

class Tooltip : public Container {
 public:
  explicit Tooltip(const std::string &elementName);

 protected:
  void renderImpl() override;

 private:
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_TOOLTIP_H
