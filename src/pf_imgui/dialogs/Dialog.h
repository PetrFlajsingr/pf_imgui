//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H
#define PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H

#include <pf_imgui/_export.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Dialog : public Renderable, public ElementContainer, public Labellable {
 public:
  Dialog(ImGuiInterface &parent, const std::string &elementName, const std::string &label, Modal modal = Modal::Yes);

  void close();

 protected:
  void renderImpl() override;

 private:
  Modal modal;
  bool closed = false;
  ImGuiInterface &owner;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_DIALOG_H
