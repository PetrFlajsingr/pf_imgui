//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOBUTTON_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOBUTTON_H

#include "interface/LabeledElement.h"
#include "interface/ResizableElement.h"
#include "interface/ValueObservableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT RadioButton : public LabeledElement, public ValueObservableElement<bool> {
 public:
  friend class RadioGroup;
  RadioButton(const std::string &elementName, const std::string &caption, bool value = false);

  [[nodiscard]] bool isSelected() const;

 protected:
  void renderImpl() override;

 private:
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_RADIOBUTTON_H
