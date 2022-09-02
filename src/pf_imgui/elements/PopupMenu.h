/**
 * @file PopupMenu.h
 * @brief Popup menu usually shown when right clicking.
 * @author Petr Flajšingr
 * @date 10.6.21
 */

#ifndef PF_IMGUI_ELEMENTS_POPUPMENU_H
#define PF_IMGUI_ELEMENTS_POPUPMENU_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/MenuItems.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Popup menu to be used for example when right clicking an element.
 */
class PF_IMGUI_EXPORT PopupMenu : public ElementWithID, public MenuContainer {
 public:
  /**
   * @brief Struct for construction of PopupMenu.
   */
  struct Config {
    using Parent = PopupMenu;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct PopupMenu
   * @param config construction args @see PopupMenu::Config
   */
  explicit PopupMenu(Config &&config);
  /**
   * Construct PopupMenu.
   * @param elementName id of the element
   */
  explicit PopupMenu(const std::string &elementName);

  /**
   * Make the menu render.
   */
  void open();

  [[nodiscard]] bool isOpen() const;
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

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_POPUPMENU_H
