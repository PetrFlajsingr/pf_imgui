/**
 * @file MenuItems.h
 * @brief Items to be used in menu bars and popup menu.
 * @author Petr Flajšingr
 * @date 10.6.21
 */
#ifndef PF_IMGUI_ELEMENTS_MENUITEMS_H
#define PF_IMGUI_ELEMENTS_MENUITEMS_H

#include <pf_imgui/Label.h>
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

class SubMenu;
class PF_IMGUI_EXPORT MenuItem : public ElementWithID {
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
class PF_IMGUI_EXPORT MenuContainer : public ElementContainer {
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
  MenuCheckboxItem &addCheckboxItem(const std::string &name, const std::string &caption, bool value = false,
                                    Persistent persistent = Persistent::No);
  /**
    * Create an instance of MenuSeparatorItem and add it to the end of children/
    * @param name ID of the MenuSeparatorItem
    * @return reference to the created MenuSeparatorItem
    */
  MenuSeparatorItem &addSeparator(const std::string &name);

  template<std::derived_from<MenuItem> T, typename... Args>
  T &addMenuItem(Args &&...args)
    requires std::constructible_from<T, Args...>
  {
    auto newItem = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = newItem.get();
    addChild(std::move(newItem));
    return *ptr;
  }

 protected:
  void renderItems();
};
/**
 * @brief An item, which can be clicked. It is basically a popup menu item.
 */
class PF_IMGUI_EXPORT MenuButtonItem : public MenuItem, public Clickable {
 public:
  /**
   * Construct MenuButtonItem
   * @param config construction args @see MenuButtonItem::Config
   */
  struct Config {
    using Parent = MenuButtonItem;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on top of the button */
  };
  /**
   * Construct MenuButtonItem
   * @param config construction args @see MenuButtonItem::Config
   */
  explicit MenuButtonItem(Config &&config);
  /**
   * Construct MenuButtonItem.
   * @param elementName ID of the element
   * @param label text rendered on the button
   */
  MenuButtonItem(const std::string &elementName, const std::string &label);

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::Button, ColorOf::ButtonHovered, ColorOf::ButtonActive,
               ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow, ColorOf::Header, ColorOf::HeaderHovered,
               ColorOf::HeaderActive>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize, StyleOf::ButtonTextAlign> style;
  Font font = Font::Default();
  Label label;

 protected:
  void renderImpl() override;
};
/**
 * @brief An item, which can be clicked and it toggles its inner value.
 */
class PF_IMGUI_EXPORT MenuCheckboxItem : public MenuItem,
                                         public ValueObservable<bool>,
                                         public Savable {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = MenuCheckboxItem;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on the item */
    bool checked = false;             /*!< Initial value */
    bool persistent = false;          /*!< Allow state saving to disk */
  };
  /**
   * Construct MenuCheckboxItem
   * @param config construction args @see MenuCheckboxItem::Config
   */
  explicit MenuCheckboxItem(Config &&config);
  /**
   * Construct MenuCheckboxItem.
   * @param elementName ID of the element
   * @param label text rendered on the button
   * @param value starting value
   */
  MenuCheckboxItem(const std::string &elementName, const std::string &label, bool value = false,
                   Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::CheckMark, ColorOf::FrameBackgroundActive,
               ColorOf::FrameBackground, ColorOf::FrameBackgroundHovered, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Label label;

 protected:
  void renderImpl() override;
};
/**
 * @brief An item, which divides menus.
 */
class PF_IMGUI_EXPORT MenuSeparatorItem : public MenuItem {
 public:
  /**
   * @brief Struct for construction of Checkbox.
   */
  struct Config {
    using Parent = MenuSeparatorItem;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct MenuSeparatorItem
   * @param config construction args @see MenuSeparatorItem::Config
   */
  explicit MenuSeparatorItem(Config &&config);
  /**
   * Construct MenuSeparatorItem.
   * @param elementName ID of the element
   */
  explicit MenuSeparatorItem(const std::string &elementName);

  ColorPalette<ColorOf::Separator, ColorOf::SeparatorHovered, ColorOf::SeparatorActive> color;

 protected:
  void renderImpl() override;
};
/**
 * @brief Basically a button for a popup menu, which contains other elements as its items.
 */
class PF_IMGUI_EXPORT SubMenu : public MenuItem, public MenuContainer {
 public:
  /**
   * @brief Struct for construction of SubMenu.
   */
  struct Config {
    using Parent = SubMenu;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on the element */
  };
  /**
   * Construct SubMenu
   * @param config construction args @see SubMenu::Config
   */
  explicit SubMenu(Config &&config);
  /**
   * Construct SubMenu.
   * @param elementName ID of the element
   * @param label text rendered on the button
   */
  SubMenu(const std::string &elementName, const std::string &label);

  Label label;

 protected:
  void renderImpl() override;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_MENUITEMS_H
