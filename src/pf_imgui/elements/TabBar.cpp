//
// Created by petr on 11/2/20.
//

#include "TabBar.h"
#include <algorithm>
#include <imgui.h>

namespace pf::ui::ig {

Tab::Tab(const std::string &elementName, const std::string &label) : ItemElement(elementName), Labellable(label) {}

void Tab::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (ImGui::BeginTabItem(getLabel().c_str())) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    ImGui::EndTabItem();
  }
}

TabBar::TabBar(const std::string &elementName, bool allowTabList)
    : Element(elementName), isTabListAllowed(allowTabList) {}

void TabBar::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto flags = isTabListAllowed ? ImGuiTabBarFlags_TabListPopupButton : ImGuiTabBarFlags{};
  if (ImGui::BeginTabBar(getName().c_str(), flags)) {
    std::ranges::for_each(tabs, [](auto &tab) { tab->render(); });
    ImGui::EndTabBar();
  }
}
Tab &TabBar::addTab(const std::string &name, const std::string &caption) {
  tabs.emplace_back(std::make_unique<Tab>(name, caption));
  return *tabs.back();
}
void TabBar::removeTab(const std::string &name) {
  if (const auto iter = std::ranges::find_if(tabs, [name](const auto &tab) { return tab->getName() == name; });
      iter != tabs.cend()) {
    tabs.erase(iter);
  }
}
bool TabBar::isTabListAllowed1() const { return isTabListAllowed; }

void TabBar::setTabListAllowed(bool tabListAllowed) { TabBar::isTabListAllowed = tabListAllowed; }

std::vector<Renderable *> TabBar::getRenderables() {
  return tabs | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); }) | ranges::to_vector;
}

}// namespace pf::ui::ig