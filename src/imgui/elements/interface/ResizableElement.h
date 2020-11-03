//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUIRESIZABLEELEMENT_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUIRESIZABLEELEMENT_H

#include "Element.h"
#include <imgui.h>

namespace pf::ui::ig {
class ResizableElement : public virtual Element {
 public:
  ResizableElement(std::string elementName, const ImVec2 &size);

  [[nodiscard]] const ImVec2 &getSize() const;
  void setSize(const ImVec2 &s);

 private:
  ImVec2 size;
};

}
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_IMGUIRESIZABLEELEMENT_H
