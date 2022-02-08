//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {
ModalDialog::ModalDialog(ImGuiInterface &parent, const std::string &elementName, std::unique_ptr<Resource<std::string>> label)
    : Renderable(elementName), Labellable(std::move(label)), Resizable(Size::Auto()), Positionable(Position{-1, -1}),
      owner(parent) {}

void ModalDialog::renderImpl() {
  if (closed) {
    owner.removeDialog(*this);
    return;
  }
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (font != nullptr) { ImGui::PushFont(font); }
  if (firstRender) { ImGui::OpenPopup(getLabel().c_str()); }
  if (ImGui::BeginPopupModal(getLabel().c_str())) {
    RAII endPopup{ImGui::EndPopup};
    std::ranges::for_each(getChildren(), &Renderable::render);
  } else {
    close();
  }
  if (font != nullptr) { ImGui::PopFont(); }
  firstRender = false;
}

void ModalDialog::close() { closed = true; }

bool ModalDialog::isClosed() const { return closed; }

void ModalDialog::setSize(const Size &newSize) {
  Resizable::setSize(newSize);  // FIXME change this to SetNextWindowSize
  ImGui::SetWindowSize(getLabel().get().c_str(), getSize().asImVec());
}
void ModalDialog::setPosition(Position pos) {
  ImGui::SetWindowPos(getLabel().get().c_str(), pos.asImVec());
  Positionable::setPosition(pos);
}
void ModalDialog::setFont(ImFont *fontPtr) { font = fontPtr; }

}  // namespace pf::ui::ig
