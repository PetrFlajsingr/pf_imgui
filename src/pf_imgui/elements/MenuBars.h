//
// Created by petr on 11/1/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_MENUBARS_H
#define PF_IMGUI_IMGUI_ELEMENTS_MENUBARS_H

#include "interface/Element.h"
#include "interface/LabeledElement.h"
#include <functional>
#include <pf_imgui/_export.h>
#include <variant>
#include <vector>

namespace pf::ui::ig {

class SubMenu;
class MenuItem;

class PF_IMGUI_EXPORT MenuContainer {
 public:
  SubMenu &addSubmenu(const std::string &name, const std::string &caption);
  MenuItem &addItem(const std::string &name, const std::string &caption);

  void removeItem(const std::string &name);

 protected:
  void renderItems();

 private:
  std::vector<std::variant<MenuItem, SubMenu>> items;
};

class PF_IMGUI_EXPORT MenuItem : public LabeledElement {
 public:
  MenuItem(const std::string &elementName, const std::string &caption);

  void setOnClick(std::invocable auto fnc) {
    onClick = fnc;
  }

 protected:
  void renderImpl() override;

 private:
  std::function<void()> onClick = [] {};
};

class PF_IMGUI_EXPORT SubMenu : public LabeledElement, public MenuContainer {
 public:
  SubMenu(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

class PF_IMGUI_EXPORT WindowMenuBar : public Element, public MenuContainer {
 public:
  explicit WindowMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;
};

class PF_IMGUI_EXPORT AppMenuBar : public Element, public MenuContainer {
 public:
  explicit AppMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_IMGUI_ELEMENTS_MENUBARS_H
