//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_BUTTON_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_BUTTON_H

#include "interface/LabeledElement.h"
#include "interface/ResizableElement.h"
#include <functional>
#include <imgui.h>

namespace pf::ui::ig {

enum class ButtonType { Normal, Small, ArrowUp, ArrowLeft, ArrowRight, ArrowDown };

// TODO: multiple listeners
class Button : public LabeledElement, public ResizableElement {
 public:
  Button(const std::string &name, std::string caption,
              ButtonType buttonType = ButtonType::Normal, const ImVec2 &size = {0, 0});
  void setOnClick(std::invocable auto fnc) { onClick = fnc; }

  [[nodiscard]] ButtonType getType() const;

  void setType(ButtonType type);

 protected:
  void renderImpl() override;

 private:
  std::function<void()> onClick = [] {};
  ButtonType type;
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_BUTTON_H
