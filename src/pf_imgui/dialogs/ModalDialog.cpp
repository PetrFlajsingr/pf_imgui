//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {
ModalDialog::ModalDialog(ImGuiInterface &parent, const std::string &elementName, const std::string &label)
    : Renderable(elementName), Labellable(label), Resizable(Size::Auto()), Positionable(ImVec2{-1, -1}), owner(parent) {
}

void ModalDialog::renderImpl() {
  if (closed) {
    owner.removeDialog(*this);
    return;
  }
  if (font != nullptr) { ImGui::PushFont(font); }
  if (firstRender) { ImGui::OpenPopup(getLabel().c_str()); }
  if (ImGui::BeginPopupModal(getLabel().c_str())) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    ImGui::EndPopup();
  } else {
    close();
  }
  if (font != nullptr) { ImGui::PopFont(); }
  firstRender = false;
}

void ModalDialog::close() { closed = true; }

bool ModalDialog::isClosed() const { return closed; }

void ModalDialog::setSize(const Size &newSize) {
  Resizable::setSize(newSize);//FIXME change this to SetNextWindowSize
  ImGui::SetWindowSize(getLabel().c_str(), getSize().asImVec());
}
void ModalDialog::setPosition(ImVec2 pos) {
  ImGui::SetWindowPos(getLabel().c_str(), pos);
  Positionable::setPosition(pos);
}
void ModalDialog::setFont(ImFont *fontPtr) { font = fontPtr; }

}// namespace pf::ui::ig