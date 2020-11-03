//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H

#include "interface/Container.h"
#include "interface/ResizableElement.h"
#include <imgui.h>

namespace pf::ui::ig {

enum class PanelLayout { Vertical, Horizontal };

class Panel : public Container, public ResizableElement {
 public:
  Panel(const std::string &elementName, std::string title,
             PanelLayout layout = PanelLayout::Vertical, const ImVec2 &panelSize = {0, 0});

 protected:
  void renderImpl() override;

 private:
  std::string title;
  PanelLayout panelLayout;
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_PANEL_H
