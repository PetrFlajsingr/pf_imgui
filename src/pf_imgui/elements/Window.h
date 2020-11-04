//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H

#include "MenuBars.h"
#include "interface/Container.h"
#include "interface/ResizableElement.h"
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

// TODO: resize, focus, collapse, position
class PF_IMGUI_EXPORT Window : public Container {
 public:
  Window(const std::string &elementName, std::string title);

  [[nodiscard]] const std::string &getTitle() const;
  void setTitle(const std::string &title);

  [[nodiscard]] WindowMenuBar &getMenuBar();

  [[nodiscard]] bool hasMenuBar() const;

 protected:
  void renderImpl() override;

 private:
  std::string title;
  std::optional<WindowMenuBar> menuBar = std::nullopt;
};

}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H
