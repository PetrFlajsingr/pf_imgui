//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TREE_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TREE_H

#include "interface/Container.h"
#include "interface/LabeledElement.h"
#include <pf_imgui/_export.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Tree : public LabeledElement, public Container {
 public:
  Tree(const std::string &elementName, const std::string &caption);

  std::shared_ptr<Tree> addNode(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TREE_H
