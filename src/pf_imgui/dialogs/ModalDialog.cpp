//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {
ModalDialog::ModalDialog(ImGuiInterface &parent, const std::string &elementName, const std::string &label)
    : Renderable(elementName), Labellable(label), Resizable(Size::Auto()), Positionable(Position{-1, -1}),
      owner(parent) {}

void ModalDialog::renderImpl() {
  if (closed) {
    owner.removeDialog(*this);
    return;
  }
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  if (font != nullptr) { ImGui::PushFont(font); }
  if (sizeDirty) {
    sizeDirty = false;
    ImGui::SetNextWindowSize(static_cast<ImVec2>(getSize()));
  }
  if (positionDirty) {
    positionDirty = false;
    ImGui::SetNextWindowPos(static_cast<ImVec2>(getPosition()));
  }
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
  sizeDirty = true;
  Resizable::setSize(newSize);
}

void ModalDialog::setPosition(Position pos) {
  positionDirty = true;
  Positionable::setPosition(pos);
}

void ModalDialog::setFont(ImFont *fontPtr) { font = fontPtr; }

}  // namespace pf::ui::ig
