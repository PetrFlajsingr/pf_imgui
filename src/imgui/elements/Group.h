//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_GROUP_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_GROUP_H

#include "interface/Container.h"
#include "interface/LabeledElement.h"

namespace pf::ui::ig {
class Group : public Container, public LabeledElement {
 public:
  Group(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};
}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_GROUP_H
