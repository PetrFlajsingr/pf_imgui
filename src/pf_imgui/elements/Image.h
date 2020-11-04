//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMAGE_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMAGE_H

#include "interface/ResizableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Image : public ResizableElement {
 public:
 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMAGE_H
