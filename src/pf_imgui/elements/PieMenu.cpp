//
// Created by xflajs00 on 11.11.2021.
//

#include "PieMenu.h"
#include <pf_imgui/details/PieMenu.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

PieItem::PieItem(const std::string &name) : Element(name) {}

PieMenuButton::PieMenuButton(const std::string &name, const std::string &label) : PieItem(name), Labellable(label) {}

void PieMenuButton::renderImpl() {
  if (ImGuiPie::PieMenuItem(getLabel().c_str()), getEnabled() == Enabled::Yes) { notifyOnClick(); }
}

std::vector<Renderable *> PieSubMenu::getRenderables() {
  return subItems | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); })
      | ranges::to_vector;
}

PieSubMenu::PieSubMenu(const std::string &name, const std::string &label) : PieItem(name), Labellable(label) {}

void PieSubMenu::renderImpl() {
  if (ImGuiPie::BeginPieMenu(getLabel().c_str(), getEnabled() == Enabled::Yes)) {
    RAII end{[] { ImGuiPie::EndPieMenu(); }};
    std::ranges::for_each(subItems, [](auto &subItem) { subItem->render(); });
  }
}

PieSubMenu &PieSubMenu::addSubMenu(const std::string &name, const std::string &label) {
  return *dynamic_cast<PieSubMenu *>(subItems.emplace_back(std::make_unique<PieSubMenu>(name, label)).get());
}

PieMenuButton &PieSubMenu::addButton(const std::string &name, const std::string &label) {
  return *dynamic_cast<PieMenuButton *>(subItems.emplace_back(std::make_unique<PieMenuButton>(name, label)).get());
}

PieMenu::PieMenu(const std::string &name) : PieSubMenu(name, name) {}

void PieMenu::renderImpl() {
  if (shouldOpen) {
    shouldOpen = false;
    ImGui::OpenPopup(getName().c_str());
    open_ = true;
  }
  if (shouldClose) {
    ImGui::CloseCurrentPopup();
    shouldClose = false;
    open_ = false;
  }
  if (open_) {
    if (ImGuiPie::BeginPiePopup(getName().c_str(), getEnabled() == Enabled::Yes)) {
      RAII end{[] { ImGuiPie::EndPiePopup(); }};
      PieSubMenu::renderImpl();
    } else {
      open_ = false;
    }
  }
}
bool PieMenu::isOpen() const { return open_; }

void PieMenu::setOpen(bool newOpen) {
  if (newOpen) {
    shouldOpen = true;
  } else {
    shouldClose = true;
  }
}
}// namespace pf::ui::ig