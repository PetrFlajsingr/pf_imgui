//
// Created by petr on 6/27/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H
#define PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H

#include <imgui.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {
enum class DockType {
  DisableCentralNode = 1 << 2,
  DisableAreaSplit = 1 << 4,
  DisableAreaResize = 1 << 5,
  AutoHideTabBar = 1 << 6
};
// TODO: viewport
// TODO: ImGui::SetNextWindowDockID - assign a window to a certain dock
// TODO: styles
class DockSpace : public Element, public Resizable {
 public:
  DockSpace(const std::string &name, const Size &s, Flags<DockType> dockFlags = Flags<DockType>{});

  void render() override;

 protected:
  void renderImpl() override;

 private:
  ImGuiDockNodeFlags flags;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_DIALOGS_DOCKSPACE_H
