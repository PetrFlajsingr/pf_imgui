/**
 * @file MenuBars.h
 * @brief All types necessary for building menu bars.
 * @author Petr Flajšingr
 * @date 1.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_MENUBARS_H
#define PF_IMGUI_ELEMENTS_MENUBARS_H

#include <functional>
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/MenuItems.h>
#include <string>
#include <variant>
#include <vector>

namespace pf::ui::ig {

// TODO: allow any element in menu
/**
 * @brief Menu bar belonging to Window.
 *
 * There can be one menu bar for each Window instance. It is created via Window::getMenuBar().
 */
class PF_IMGUI_EXPORT WindowMenuBar : public Element, public MenuContainer {
 public:
  /**
   * Construct WindowMenuBar.
   * @param elementName ID of the element
   */
  explicit WindowMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;
};
/**
 * @brief Menu bar belonging to the entire application window.
 *
 * Only one of these can exist at a time. It is created via ImGuiInterface::getMenuBar().
 */
class PF_IMGUI_EXPORT AppMenuBar : public Element, public MenuContainer {
 public:
  /**
   * Construct AppMenuBar.
   * @param elementName ID of the element
   */
  explicit AppMenuBar(const std::string &elementName);

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_MENUBARS_H
