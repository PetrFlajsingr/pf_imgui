//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

ModalDialog::ModalDialog(const std::string &elementName, const std::string &labelText, Size dialogSize)
    : Renderable(elementName), Resizable(dialogSize), position(Position{-1, -1}), label(labelText),
      sizeDirty(dialogSize != Size::Auto()) {
  position.addListener([this](auto) { positionDirty = true; });
}

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
    ImGui::SetNextWindowPos(static_cast<ImVec2>(*position));
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

}  // namespace pf::ui::ig
