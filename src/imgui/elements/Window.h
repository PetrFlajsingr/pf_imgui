//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H

#include "MenuBars.h"
#include "interface/Container.h"
#include "interface/ResizableElement.h"

namespace pf::ui::ig {

// TODO: resize, focus, collapse, position
class Window : public Container {
 public:
  Window(const std::string &elementName, std::string title);

  [[nodiscard]] const std::string &getTitle() const;
  void setTitle(const std::string &title);

  [[nodiscard]] ImGuiWindowMenuBar &getMenuBar();

  [[nodiscard]] bool hasMenuBar() const;
 protected:
  void renderImpl() override;

 private:
  std::string title;
  std::optional<ImGuiWindowMenuBar> menuBar = std::nullopt;
};

}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_WINDOW_H
