//
// Created by petr on 11/1/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_MENUBARS_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_MENUBARS_H

#include "interface/Element.h"
#include "interface/LabeledElement.h"
#include <functional>
#include <variant>
#include <vector>

namespace pf::ui::ig {

class ImGuiSubMenu;
class ImGuiMenuItem;

class ImGuiMenuContainer{
 public:
  ImGuiSubMenu &addSubmenu(const std::string &name, const std::string &caption);
  ImGuiMenuItem &addItem(const std::string &name, const std::string &caption);

  void removeItem(const std::string &name);
 protected:
  void renderItems();

 private:
  std::vector<std::variant<ImGuiMenuItem, ImGuiSubMenu>> items;
};


class ImGuiMenuItem : public LabeledElement {
 public:
  ImGuiMenuItem(const std::string &elementName, const std::string &caption);

  void setOnClick(std::invocable auto fnc) {
    onClick = fnc;
  }

 protected:
  void renderImpl() override;

 private:
  std::function<void()> onClick = []{};
};

class ImGuiSubMenu : public LabeledElement, public ImGuiMenuContainer {
 public:
  ImGuiSubMenu(const std::string &elementName, const std::string &caption);
 protected:
  void renderImpl() override;

};

class ImGuiWindowMenuBar : public Element, public ImGuiMenuContainer {
 public:
  explicit ImGuiWindowMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;

};

class ImGuiAppMenuBar : public Element, public ImGuiMenuContainer {
 public:
  explicit ImGuiAppMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;

};


}

#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_MENUBARS_H
