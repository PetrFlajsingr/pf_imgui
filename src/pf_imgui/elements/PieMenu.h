//
// Created by xflajs00 on 11.11.2021.
//

#ifndef PF_IMGUI_PIEMENU_H
#define PF_IMGUI_PIEMENU_H

#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/RenderablesContainer.h>
#include <pf_imgui/unique_id.h>

namespace pf::ui::ig {

/**
 * @brief A utility type for storing pie menu items.
 */
class PieItem : public Element {
 public:
  /**
   *
   * @param name unique element name
   */
  explicit PieItem(const std::string &name);
};

/**
 * @brief A pie menu item functioning as a button and a leaf of the menu tree.
 */
class PieMenuButton : public PieItem, public Labellable, public Clickable {
 public:
  /**
   *
   * @param name unique element name
   * @param label text rendered on the button
   */
  PieMenuButton(const std::string &name, const std::string &label);

 protected:
  void renderImpl() override;

 private:
};

/**
 * @brief An item of pie menu, which contains other items.
 */
class PieSubMenu : public PieItem, public Labellable, public RenderablesContainer {
 public:
  /**
   *
   * @param name unique element name
   * @param label text rendered on the item
   */
  PieSubMenu(const std::string &name, const std::string &label);

  PieSubMenu &addSubMenu(const std::string &name, const std::string &label);
  PieMenuButton &addButton(const std::string &name, const std::string &label);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::unique_ptr<PieItem>> subItems;
};

/**
 * @brief A root of pie menu, contains either more menus or clickable items.
 */
class PieMenu : public PieSubMenu {
 public:
  /**
   *
   * @param name unique name of the element
   */
  explicit PieMenu(const std::string &name);

  [[nodiscard]] bool isOpen() const;

  void setOpen(bool newOpen);

 protected:
  void renderImpl() override;

 private:
  bool open_ = false;
  bool shouldOpen = false;
  bool shouldClose = false;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_PIEMENU_H
