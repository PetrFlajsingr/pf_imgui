//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_IMGUIELEMENT_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_IMGUIELEMENT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Element {
 public:
  explicit Element(std::string elementName);
  virtual ~Element() = default;
  [[nodiscard]] const std::string &getName() const;

  [[nodiscard]] Visibility getVisibility() const;
  void setVisibility(Visibility visibility);

  void render();

 protected:
  virtual void renderImpl() = 0;

 private:
  std::string name;
  Visibility visibility = Visibility::Visible;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_IMGUIELEMENT_H
