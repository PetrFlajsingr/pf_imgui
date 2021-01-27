//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H
#define PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H

#include "MenuBars.h"
#include "interface/ElementContainer.h"
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
                               public ElementContainer,
                               public Focusable,
                               public Hoverable,
                               public Collapsible,
                               public Resizable,
                               public Positionable, public Labellable {
 public:
  Window(std::string name, std::string label, AllowCollapse allowCollapse = AllowCollapse::No, Persistent persistent = Persistent::No);
  Window(std::string name, std::string label, Persistent persistent);


  [[nodiscard]] WindowMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;
  void removeMenuBar();

  void setSize(const ImVec2 &size) override;

  void render() override;

  void setFocus() override;
  void setCollapsed(bool collapsed) override;
  void setPosition(ImVec2 pos) override;

 protected:
  void renderImpl() override;

 private:
  std::unique_ptr<WindowMenuBar> menuBar = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H
