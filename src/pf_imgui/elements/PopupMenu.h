/**
 * @file PopupMenu.h
 * @brief Popup menu usually shown when right clicking.
 * @author Petr Flajšingr
 * @date 10.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_POPUPMENU_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_POPUPMENU_H

#include <pf_imgui/_export.h>
#include <pf_imgui/elements/MenuItems.h>
#include <pf_imgui/interface/Element.h>

namespace pf::ui::ig {

/**
 * @brief Popup menu to be used for example when right clicking an element.
 */
class PopupMenu : public Element, public MenuContainer {
 public:
  /**
   * Construct PopupMenu.
   * @param name id of the element
   */
  explicit PopupMenu(const std::string &name);

  /**
   * Make the menu render.
   */
  void open();
  /**
   * Make the menu stop rendering.
   */
  void close();

 protected:
  void renderImpl() override;

 private:
  bool opened = false;
  bool firstRender = true;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_POPUPMENU_H