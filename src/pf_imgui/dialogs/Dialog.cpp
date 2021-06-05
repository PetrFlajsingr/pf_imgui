//
// Created by petr on 10/31/20.
//

#include "Dialog.h"
#include <imgui.h>
#include <pf_imgui/ImGuiInterface.h>

namespace pf::ui::ig {
Dialog::Dialog(ImGuiInterface &parent, const std::string &elementName, const std::string &label, Modal modal)
    : Renderable(elementName), Labellable(label), Resizable(Size::Auto()), Positionable(ImVec2{-1, -1}), modal(modal),
      owner(parent) {}

void Dialog::renderImpl() {
  if (closed) {
    owner.removeDialog(*this);
    return;
  }
  if (firstRender) { ImGui::OpenPopup(getLabel().c_str()); }
  bool open;
  if (modal == Modal::Yes) {
    open = ImGui::BeginPopupModal(getLabel().c_str());
  } else {
    open = ImGui::BeginPopup(getLabel().c_str());
  }
  if (open) {
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
    ImGui::EndPopup();
  } else {
    close();
  }
  firstRender = false;
}

void Dialog::close() { closed = true; }

bool Dialog::isClosed() const { return closed; }

void Dialog::setSize(const Size &newSize) {
  Resizable::setSize(newSize);//FIXME change this to SetNextWindowSize
  ImGui::SetWindowSize(getLabel().c_str(), getSize().asImVec());
}
void Dialog::setPosition(ImVec2 pos) {
  ImGui::SetWindowPos(getLabel().c_str(), pos);
  Positionable::setPosition(pos);
}

}// namespace pf::ui::ig