//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TEXT_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TEXT_H

#include "interface/Element.h"

namespace pf::ui::ig {
// TODO: color
class Text : public virtual Element {
 public:
  Text(const std::string &elementName, std::string text);

  [[nodiscard]] const std::string &getText() const;
  void setText(const std::string &text);

 protected:
  void renderImpl() override;

 private:
  std::string text;
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_TEXT_H
