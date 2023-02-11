//
// Created by petr on 10/31/20.
//

#include "ModalDialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {

ModalDialog::ModalDialog(std::string_view elementName, std::string_view labelText, Size dialogSize)
    : Renderable(elementName), label(std::string{labelText}), position(Position{-1, -1}), size(dialogSize),
      sizeDirty(dialogSize != Size::Auto()) {
  position.addListener([this](auto) { positionDirty = true; });
  size.addListener([this](auto) { sizeDirty = true; });
}

void ModalDialog::renderImpl() {
  if (closed) { return; }
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (sizeDirty) {
    sizeDirty = false;
    ImGui::SetNextWindowSize(static_cast<ImVec2>(*size));
  }
  if (positionDirty) {
    positionDirty = false;
    ImGui::SetNextWindowPos(static_cast<ImVec2>(*position));
  }
  if (firstRender) { ImGui::OpenPopup(label->get().c_str()); }
  if (ImGui::BeginPopupModal(label->get().c_str())) {
    ScopeExit endPopup{&ImGui::EndPopup};
    std::ranges::for_each(getChildren(), &Renderable::render);
  } else {
    close();
  }
  firstRender = false;
}

void ModalDialog::close() { closed = true; }

bool ModalDialog::isClosed() const { return closed; }

}  // namespace pf::ui::ig
