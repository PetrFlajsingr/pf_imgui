//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H

#include "interface/Container.h"
#include "interface/ResizableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Panel : public Container, public ResizableElement {
 public:
  Panel(const std::string &elementName, std::string title,
        PanelLayout layout = PanelLayout::Vertical, const ImVec2 &panelSize = {0, 0});

 protected:
  void renderImpl() override;

 private:
  std::string title;
  PanelLayout panelLayout;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H
