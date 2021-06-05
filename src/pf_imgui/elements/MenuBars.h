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
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <variant>
#include <vector>

namespace pf::ui::ig {

class SubMenu;
class PF_IMGUI_EXPORT MenuItem : public Element {
 public:
  explicit MenuItem(const std::string &name);

  /**
   * Set if the parent menu is closed on interact - the menu might still get closed in some cases.
   * @param close true if the menu should close, false if not
   */
  void setCloseOnInteract(bool close);
  /**
   * Check if parent menu is closed on interact.
   * @return true if the menu closes, false if not
   */
  [[nodiscard]] bool isCloseMenuOnInteract() const;

  void render() override;

 private:
  bool closeOnInteract = true;
};
class MenuButtonItem;
class MenuCheckboxItem;
class MenuSeparatorItem;
/**
 * @brief An item which can contain other menus.
 */
class PF_IMGUI_EXPORT MenuContainer {
 public:
  /**
   * Create an instance of SubMenu and add it to the end if children.
   * @param name ID of the SubMenu
   * @param caption text of the SubMenu
   * @return reference to the created SubMenu
   */
  SubMenu &addSubmenu(const std::string &name, const std::string &caption);
  /**
   * Create an instance of MenuButtonItem and add it to the end of children/
   * @param name ID of the MenuButtonItem
   * @param caption text of the MenuButtonItem
   * @return reference to the created MenuButtonItem
   */
  MenuButtonItem &addButtonItem(const std::string &name, const std::string &caption);
  /**
   * Create an instance of MenuCheckboxItem and add it to the end of children/
   * @param name ID of the MenuCheckboxItem
   * @param caption text of the MenuButtonItem
   * @param value starting value
   * @return reference to the created MenuButtonItem
   */
  MenuCheckboxItem &addCheckboxItem(const std::string &name, const std::string &caption, bool value = false);
  /**
   * Create an instance of MenuSeparatorItem and add it to the end of children/
   * @param name ID of the MenuSeparatorItem
   * @return reference to the created MenuSeparatorItem
   */
  MenuSeparatorItem &addSeparator(const std::string &name);

  template<std::derived_from<MenuItem> T, typename... Args>
  T &addItem(Args &&...args) requires std::constructible_from<T, Args...> {
    auto newItem = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = newItem.get();
    items.emplace_back(std::move(newItem));
    return *ptr;
  }

  /**
   * Remove item by ID.
   * @param name ID of the item to be removed
   */
  void removeItem(const std::string &name);

 protected:
  void renderItems();

 private:
  std::vector<std::unique_ptr<MenuItem>> items;
};
/**
 * @brief An item, which can be clicked. It is basically a popup menu item.
 */
class PF_IMGUI_EXPORT MenuButtonItem : public MenuItem, public Labellable, public Clickable {
 public:
  /**
   * Construct MenuButtonItem.
   * @param elementName ID of the element
   * @param label text rendered on the button
   */
  MenuButtonItem(const std::string &elementName, const std::string &label);

 protected:
  void renderImpl() override;
};
/**
 * @brief An item, which can be clicked and it toggles its inner value.
 */
class PF_IMGUI_EXPORT MenuCheckboxItem : public MenuItem,
                                         public Labellable,
                                         public ValueObservable<bool>,
                                         public Savable {
 public:
  /**
   * Construct MenuCheckboxItem.
   * @param elementName ID of the element
   * @param label text rendered on the button
   * @param value starting value
   */
  MenuCheckboxItem(const std::string &elementName, const std::string &label, bool value = false,
                   Persistent persistent = Persistent::No);

 protected:
  void renderImpl() override;
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;
};
/**
 * @brief An item, which divides menus.
 */
class PF_IMGUI_EXPORT MenuSeparatorItem : public MenuItem {
 public:
  /**
   * Construct MenuSeparatorItem.
   * @param elementName ID of the element
   */
  MenuSeparatorItem(const std::string &elementName);

 protected:
  void renderImpl() override;
};
/**
 * @brief Basically a button for a popup menu, which contains other elements as its items.
 */
class PF_IMGUI_EXPORT SubMenu : public MenuItem, public Labellable, public MenuContainer {
 public:
  /**
   * Construct SubMenu.
   * @param elementName ID of the element
   * @param label text rendered on the button
   */
  SubMenu(const std::string &elementName, const std::string &label);

 protected:
  void renderImpl() override;
};
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

}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_MENUBARS_H
