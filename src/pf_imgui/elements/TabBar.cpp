//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>

namespace pf::ui::ig {

Tab::Tab(const std::string &elementName, const std::string &caption)
    : Element(elementName), LabeledElement(elementName, caption), Container(elementName) {}

void Tab::renderImpl() {
  if (ImGui::BeginTabItem(getLabel().c_str())) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.get().render(); });
    ImGui::EndTabItem();
  }
}

TabBar::TabBar(const std::string &elementName) : Element(elementName) {}

void TabBar::renderImpl() {
  if (ImGui::BeginTabBar(getName().c_str())) {
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
    ImGui::EndTabBar();
  }
}
std::shared_ptr<Tab> TabBar::addTab(const std::string &name, const std::string &caption) {
  tabs.emplace_back(std::make_shared<Tab>(name, caption));
  return tabs.back();
}
void TabBar::removeTab(const std::string &name) {
  if (const auto iter =
          std::ranges::find_if(tabs, [name](const auto &tab) { return tab->getName() == name; });
      iter != tabs.cend()) {
    tabs.erase(iter);
  }
}
}// namespace pf::ui::ig