//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
#define PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H

#include "interface/ResizableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Image : public ResizableElement {
 public:
 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
