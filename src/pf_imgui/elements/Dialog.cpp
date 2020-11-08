//
// Created by petr on 10/31/20.
//

#include "Dialog.h"
#include <imgui.h>

namespace pf::ui::ig {
Dialog::Dialog(Container &parent, const std::string &elementName, const std::string &caption,
               Modal modal)
    : Element(elementName), Container(elementName), LabeledElement(elementName, caption),
      modal(modal), owner(parent) {}

void Dialog::renderImpl() {
  if (closed) {
    owner.enqueueChildRemoval(getName());
    return;
  }
  ImGui::OpenPopup(getLabel().c_str());
  auto open = bool{};
  if (modal == Modal::Yes) {
    open = ImGui::BeginPopupModal(getLabel().c_str());
  } else {
    open = ImGui::BeginPopup(getLabel().c_str());
  }
  if (open) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.get().render(); });
    ImGui::EndPopup();
  }
  if (closed) { owner.enqueueChildRemoval(getName()); }
}
void Dialog::close() { closed = true; }

}// namespace pf::ui::ig