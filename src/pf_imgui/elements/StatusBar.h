//
// Created by xflajs00 on 15.11.2021.
//

#ifndef PF_IMGUI_STATUSBAR_H
#define PF_IMGUI_STATUSBAR_H

#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ElementContainer.h>

namespace pf::ui::ig {

/**
 * @brief A status bar rendered on the bottom of the main window.
 */
class AppStatusBar : public Element, public ElementContainer {
 public:
  /**
   * Construct AppStatusBar.
   * @param name unique name of the element
   * @param height height of the status bar
   */
  explicit AppStatusBar(const std::string &name, float height = ImGui::GetFrameHeight());

 protected:
  void renderImpl() override;

 private:
  float height;
};

}
#endif//PF_IMGUI_STATUSBAR_H
