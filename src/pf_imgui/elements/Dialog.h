//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H
#define PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H

#include "interface/Container.h"
#include "interface/LabeledElement.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Dialog : public Container, public LabeledElement {
 public:
  Dialog(Container &parent, const std::string &elementName, const std::string &caption, Modal modal = Modal::Yes);

  void close();

 protected:
  void renderImpl() override;

 private:
  Modal modal;
  bool closed = false;
  Container &owner;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H
