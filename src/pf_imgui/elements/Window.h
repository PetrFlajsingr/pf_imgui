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
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Window : public Renderable,
                               public Container,
                               public Focusable,
                               public Hoverable,
                               public Collapsible,
                               public Resizable {
 public:
  Window(std::string elementName, std::string title);

  [[nodiscard]] const std::string &getTitle() const;
  void setTitle(const std::string &title);

  [[nodiscard]] WindowMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;
  void removeMenuBar();

  [[nodiscard]] const ImVec2 &getPosition() const;
  void setPosition(const ImVec2 &position);

  void setSize(const ImVec2 &size) override;

  void render() override;

  [[nodiscard]] const std::string &getName() const;

 protected:
  void renderImpl() override;
  void setFocus_impl() override;
  void collapse_impl(bool collapse) override;

 private:
  std::string name;
  std::string title;
  std::unique_ptr<WindowMenuBar> menuBar = nullptr;
  ImVec2 position;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_WINDOW_H
