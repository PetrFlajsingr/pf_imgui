//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H
#define PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H

#include "interface/Container.h"
#include "interface/Element.h"
#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Tab : public ItemElement, public Labellable, public Container {
 public:
  Tab(const std::string &elementName, const std::string &label);

 protected:
  void renderImpl() override;
};

class PF_IMGUI_EXPORT TabBar : public Element {
 public:
  explicit TabBar(const std::string &elementName);

  Tab &addTab(const std::string &name, const std::string &caption);
  void removeTab(const std::string &name);

 protected:
  void renderImpl() override;

 private:
  std::vector<std::unique_ptr<Tab>> tabs;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H
