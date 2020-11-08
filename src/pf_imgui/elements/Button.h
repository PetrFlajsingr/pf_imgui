//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_BUTTON_H
#define PF_IMGUI_ELEMENTS_BUTTON_H

#include "interface/LabeledElement.h"
#include "interface/ResizableElement.h"
#include <functional>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

// TODO: multiple listeners
class PF_IMGUI_EXPORT Button : public LabeledElement, public ResizableElement {
 public:
  Button(const std::string &name, std::string caption, ButtonType buttonType = ButtonType::Normal,
         const ImVec2 &size = {0, 0});
  void setOnClick(std::invocable auto fnc) { onClick = fnc; }

  [[nodiscard]] ButtonType getType() const;

  void setType(ButtonType type);

 protected:
  void renderImpl() override;

 private:
  std::function<void()> onClick = [] {};
  ButtonType type;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_BUTTON_H
