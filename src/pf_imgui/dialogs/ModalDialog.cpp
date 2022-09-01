//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

ModalDialog::ModalDialog(const std::string &elementName, const std::string &label, Size dialogSize)
    : Renderable(elementName), Resizable(dialogSize), Positionable(Position{-1, -1}), label(label),
      sizeDirty(dialogSize != Size::Auto()) {}

void ModalDialog::renderImpl() {
  if (closed) { return; }
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (sizeDirty) {
    sizeDirty = false;
    ImGui::SetNextWindowSize(static_cast<ImVec2>(getSize()));
  }
  if (positionDirty) {
    positionDirty = false;
    ImGui::SetNextWindowPos(static_cast<ImVec2>(getPosition()));
  }
  if (firstRender) { ImGui::OpenPopup(label.get().c_str()); }
  if (ImGui::BeginPopupModal(label.get().c_str())) {
    RAII endPopup{ImGui::EndPopup};
    std::ranges::for_each(getChildren(), &Renderable::render);
  } else {
    close();
  }
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

}  // namespace pf::ui::ig
