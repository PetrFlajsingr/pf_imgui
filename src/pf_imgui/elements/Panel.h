//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_PANEL_H
#define PF_IMGUI_IMGUI_ELEMENTS_PANEL_H

#include "interface/Container.h"
#include "interface/Resizable.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Panel : public Container, public Resizable {
 public:
  Panel(const std::string &elementName, std::string title, PanelLayout layout = PanelLayout::Vertical,
        const ImVec2 &panelSize = {0, 0});

 protected:
  void renderImpl() override;

 private:
  std::string title;
  PanelLayout panelLayout;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_PANEL_H
