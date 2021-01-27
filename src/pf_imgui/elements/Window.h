//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H
#define PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H

#include "MenuBars.h"
#include "interface/Container.h"
#include "interface/Resizable.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Collapsible.h>
#include <pf_imgui/elements/interface/Focusable.h>
#include <pf_imgui/elements/interface/Hoverable.h>
#include <pf_imgui/elements/interface/Positionable.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Window : public Renderable,
                               public Container,
                               public Focusable,
                               public Hoverable,
                               public Collapsible,
                               public Resizable,
                               public Positionable {
 public:
  Window(std::string name, std::string title, AllowCollapse allowCollapse = AllowCollapse::No);

  [[nodiscard]] const std::string &getTitle() const;
  void setTitle(const std::string &title);

  [[nodiscard]] WindowMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;
  void removeMenuBar();

  void setSize(const ImVec2 &size) override;

  void render() override;

  [[nodiscard]] const std::string &getName() const;

  void setFocus() override;

  void setCollapsed(bool collapsed) override;

  void setPosition(ImVec2 pos) override;

 protected:
  void renderImpl() override;

 protected:
 private:
  std::string name;
  std::string title;
  std::unique_ptr<WindowMenuBar> menuBar = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H
