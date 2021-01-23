//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_GROUP_H
#define PF_IMGUI_IMGUI_ELEMENTS_GROUP_H

#include "interface/Collapsible.h"
#include "interface/Container.h"
#include "interface/LabeledElement.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Group : public Container, public LabeledElement, public Collapsible {
 public:
  Group(const std::string &elementName, const std::string &caption, bool allowCollapse = false);

 protected:
  void renderImpl() override;
  void collapse_impl(bool) override;
 private:
  bool collapsible;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_GROUP_H
