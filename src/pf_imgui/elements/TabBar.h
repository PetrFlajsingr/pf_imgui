/**
* @file TabBar.h
* @brief TabBar element.
* @author Petr Flajšingr
* @date 2.11.20
*/

#ifndef PF_IMGUI_ELEMENTS_TABBAR_H
#define PF_IMGUI_ELEMENTS_TABBAR_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

// @todo: TabBar flags

enum class TabMod {
  DisplayDot = ImGuiTabItemFlags_UnsavedDocument,
  DisableMidMouseClose = ImGuiTabItemFlags_NoCloseWithMiddleMouseButton,
  ForceLeft = ImGuiTabItemFlags_Leading,
  ForceRight = ImGuiTabItemFlags_Trailing
};

/**
 * @brief A button which looks like a Tab in TabBar.
 */
class PF_IMGUI_EXPORT TabButton : public ItemElement, public Labellable, public Clickable {
 public:
  TabButton(const std::string &elementName, const std::string &label, const Flags<TabMod> &mods = Flags<TabMod>{});

  void setMods(const Flags<TabMod> &mods);

 protected:
  void renderImpl() override;

  ImGuiTabItemFlags flags;
};

/**
* @brief Par of TabBar, when clicked it switches to the selected sub-layout.
 *
 * @todo: close & selected persistence
*/
class PF_IMGUI_EXPORT Tab : public TabButton, public ElementContainer {
 public:
  /**
  * Construct Tab.
  * @param elementName ID of the Tab
  * @param mods modifiers
  * @param label text rendered on the Tab
  */
  Tab(const std::string &elementName, const std::string &label, const Flags<TabMod> &mods = Flags<TabMod>{},
      bool closeable = false);
  /**
  * Construct Tab.
  * @param elementName ID of the Tab
  * @param label text rendered on the Tab
  */
  Tab(const std::string &elementName, const std::string &label, bool closeable = false);

  ~Tab() override;

  /**
  * Called when a Tab's contents are in/visible.
  * @param listener
  */
  void addOpenListener(std::invocable<bool> auto &&listener) {
    openObservable.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
  *
  * @return true if the Tab is currently open
  */
  [[nodiscard]] bool isOpen() const;

  [[nodiscard]] bool isSelected() const;

  void setOpen(bool newOpen);

  void setSelected();

 protected:
  void renderImpl() override;

 private:
  Observable_impl<bool> openObservable;
  Observable_impl<bool> selectedObservable;
  bool *open;
  bool selected = false;
  bool setSelectedInNextFrame = false;
};

/**
* @brief A bar for containment and switching of tabs.
*
* TabBar contains only Bars, elements are inside the Tabs created by addTab().
*
*/
class PF_IMGUI_EXPORT TabBar : public Element, public RenderablesContainer {
 public:
  enum class ResizePolicy {
    ResizeDown = ImGuiTabBarFlags_FittingPolicyResizeDown,
    Scroll = ImGuiTabBarFlags_FittingPolicyScroll
  };
  /**
  * Construct TabBar.
  * @param elementName ID of the TabBar
  * @param allowTabList set if tab list on the left side is allowed
  */
  explicit TabBar(const std::string &elementName, bool allowTabList = false);

  /**
  * Create a new Tab.
  * @param tabName ID of the Tab
  * @param caption text rendered on the Tab
  * @param mods modifiers
  * @return reference to the newly created Tab
  */
  Tab &addTab(const std::string &tabName, const std::string &caption, const Flags<TabMod> &mods = Flags<TabMod>{},
              bool closeable = false);
  /**
  * Create a new Tab.
  * @param tabName ID of the Tab
  * @param caption text rendered on the Tab
  * @return reference to the newly created Tab
  */
  Tab &addTab(const std::string &tabName, const std::string &caption, bool closeable);
  /**
  * Create a new TabButton.
  * @param buttonName ID of the TabButton
  * @param caption text rendered on the TabButton
  * @return reference to the newly created TabButton
  */
  TabButton &addTabButton(const std::string &buttonName, const std::string &caption,
                          const Flags<TabMod> &mods = Flags<TabMod>{});
  /**
  * Remove a tab with the given ID.
  * If no such Tab exists nothing happens.
  * @param tabName ID of the tab to be removed
  */
  void removeTab(const std::string &tabName);

  /**
  * Get currently selected Tab.
  * @return
  */
  [[nodiscard]] Tab &getSelectedTab();

  /**
  * Set Tab as selected. If a Tab with this name is not found nothing happens.
  * @param tabName name of the tab to open
  */
  void setSelectedTab(std::string_view tabName);

  /**
  * Check if tab list on the left side is allowed.
  * @return true if allowed
  */
  [[nodiscard]] bool isTabListAllowed() const;
  /**
  * Set if tab list on the left side is allowed.
  * @param listAllowed allowed
  */
  void setTabListAllowed(bool listAllowed);

  /**
   * Check if tab reordering is allowed.
   * @return  true if allowd
   */
  [[nodiscard]] bool isReorderable() const;

  /**
   * Enable/disable user tab reodering
   * @param reorderable
   */
  void setReorderable(bool reorderable);

  /**
   * Check if the user can close tabs with middle mouse button.
   * @return
   */
  [[nodiscard]] bool isCloseMidMouseButton() const;

  /**
   * Enable/disable tab closing with middle mouse button.
   * @param allowedClose
   */
  void setCloseMidMouseButton(bool allowedClose);

  /**
   * Get resizing policy, which is used when tabs can no longer fit in their default form.
   * @return
   */
  [[nodiscard]] Flags<ResizePolicy> getResizePolicy() const;

  /**
   * Set resizing policy, which controls how tabs get resized when there's no available space.
   * @param newPolicy
   */
  void setResizePolicy(const Flags<ResizePolicy> &newPolicy);

  [[nodiscard]] inline auto getTabButtons() const {
    return tabs | std::views::transform([](auto &btn) -> TabButton & { return *btn; });
  }

  [[nodiscard]] inline auto getTabs() const {
    return tabs | std::views::filter([](auto &tabButton) { return dynamic_cast<Tab *>(tabButton.get()) != nullptr; })
        | std::views::transform([](auto &tab) -> Tab & { return *dynamic_cast<Tab *>(tab.get()); });
  }

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::unique_ptr<TabButton>> tabs;
  ImGuiTabBarFlags flags = 0;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_TABBAR_H
