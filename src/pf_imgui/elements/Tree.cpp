//
// Created by petr on 10/31/20.
//

#include "Tree.h"
#include <imgui.h>

namespace pf::ui::ig {

details::TreeRecord::TreeRecord(const std::string &elementName, const std::string &label,
                                Flags<ImGuiTreeNodeFlags_> defaultFlags)
    : ItemElement(elementName), Labellable(label), flags(defaultFlags) {}

TreeLeaf::TreeLeaf(const std::string &elementName, const std::string &label, bool selected, Persistent persistent)
    : TreeRecord(elementName, label, ImGuiTreeNodeFlags_Leaf), ValueObservable(selected), Savable(persistent) {
  if (selected) {
    flags |= ImGuiTreeNodeFlags_Selected;
  } else {
    flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Selected);
  }
}

void TreeLeaf::setValue(const bool &newValue) {
  if (newValue == getValue()) { return; }
  if (newValue) {
    flags |= ImGuiTreeNodeFlags_Selected;
  } else {
    flags &= static_cast<ImGuiTreeNodeFlags_>(~ImGuiTreeNodeFlags_Selected);
  }
  if (limiter != nullptr && newValue) { limiter->selected = this; }
  ValueObservable::setValue(newValue);
}

void TreeLeaf::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  const auto pop = ImGui::TreeNodeEx(getLabel().c_str(), *flags);
  RAII end{[pop] {
    if (pop) { ImGui::TreePop(); }
  }};
  if (ImGui::IsItemClicked()) { setValue(!getValue()); }
  if (limiter != nullptr && limiter->selected != this) { setValue(false); }
}

void TreeLeaf::unserialize_impl(const toml::table &src) { setValue(src["value"].value_or(false)); }

toml::table TreeLeaf::serialize_impl() { return toml::table{{{"value", getValue()}}}; }

}// namespace pf::ui::ig