//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H

#include "Element.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT LabeledElement : public virtual Element {
 public:
  LabeledElement(std::string elementName, std::string caption);

  [[nodiscard]] const std::string &getLabel() const;
  void setLabel(const std::string &cap);

 private:
  std::string label;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
