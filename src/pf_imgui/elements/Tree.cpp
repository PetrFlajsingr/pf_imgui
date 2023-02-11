//
// Created by petr on 10/31/20.
//

#include "Tree.h"
#include <imgui.h>

namespace pf::ui::ig {

details::TreeRecord::TreeRecord(std::string_view elementName, std::string_view treeLabel, Flags<ImGuiTreeNodeFlags_> defaultFlags)
    : ItemElement(elementName), label(std::string{treeLabel}), flags(defaultFlags) {}

TreeLeaf::TreeLeaf(TreeLeaf::Config &&config)
    : TreeLeaf(config.name, config.label, config.selected, config.persistent ? Persistent::Yes : Persistent::No) {}

TreeLeaf::TreeLeaf(std::string_view elementName, std::string_view labelText, bool initialValue, Persistent persistent)
    : TreeRecord(elementName, labelText, ImGuiTreeNodeFlags_Leaf), Savable(persistent), selected(initialValue) {
  if (*selected) {
    flags |= ImGuiTreeNodeFlags_Selected;
  } else {
    flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Selected);
  }
  selected.addListener([this](const auto newValue) {
    if (newValue) {
      flags |= ImGuiTreeNodeFlags_Selected;
    } else {
      flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Selected);
    }
    if (limiter != nullptr && newValue) { limiter->selected = this; }
  });
}

toml::table TreeLeaf::toToml() const { return toml::table{{"value", getValue()}}; }

void TreeLeaf::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<bool>(); newVal.has_value()) { *selected.modify() = *newVal; }
  }
}

void TreeLeaf::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto pop = ImGui::TreeNodeEx(label->get().c_str(), *flags);
  ScopeExit end{[pop] {
    if (pop) { ImGui::TreePop(); }
  }};
  if (ImGui::IsItemClicked()) { *selected.modify() = !*selected; }
  if (limiter != nullptr && limiter->selected != this) { *selected.modify() = false; }
}

const bool &TreeLeaf::getValue() const { return *selected; }

void TreeLeaf::setValue(const bool &newValue) { *selected.modify() = newValue; }

Subscription TreeLeaf::addValueListenerImpl(std::function<void(const bool &)> listener) {
  return selected.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
