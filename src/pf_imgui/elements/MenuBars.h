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
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class ImGuiSubMenu;
class ImGuiMenuItem;

class PF_IMGUI_EXPORT ImGuiMenuContainer{
 public:
  ImGuiSubMenu &addSubmenu(const std::string &name, const std::string &caption);
  ImGuiMenuItem &addItem(const std::string &name, const std::string &caption);

  void removeItem(const std::string &name);
 protected:
  void renderItems();

 private:
  std::vector<std::variant<ImGuiMenuItem, ImGuiSubMenu>> items;
};


class PF_IMGUI_EXPORT ImGuiMenuItem : public LabeledElement {
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

class PF_IMGUI_EXPORT ImGuiSubMenu : public LabeledElement, public ImGuiMenuContainer {
 public:
  ImGuiSubMenu(const std::string &elementName, const std::string &caption);
 protected:
  void renderImpl() override;

};

class PF_IMGUI_EXPORT ImGuiWindowMenuBar : public Element, public ImGuiMenuContainer {
 public:
  explicit ImGuiWindowMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;

};

class PF_IMGUI_EXPORT ImGuiAppMenuBar : public Element, public ImGuiMenuContainer {
 public:
  explicit ImGuiAppMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;

};


}

#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_MENUBARS_H
