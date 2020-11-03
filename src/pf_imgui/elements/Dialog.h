//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DIALOG_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DIALOG_H

#include "interface/Container.h"
#include "interface/LabeledElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

enum class Modal { Yes, No };

class PF_IMGUI_EXPORT Dialog : public Container, public LabeledElement {
 public:
  Dialog(Container &parent, const std::string &elementName, const std::string &caption,
              Modal modal = Modal::Yes);

  void close();

 protected:
  void renderImpl() override;

 private:
  Modal modal;
  bool closed = false;
  Container &owner;
};
}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_DIALOG_H
