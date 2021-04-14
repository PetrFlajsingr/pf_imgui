//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H
#define PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Tab : public ItemElement, public Labellable, public ElementContainer {
 public:
  Tab(const std::string &elementName, const std::string &label);

 protected:
  void renderImpl() override;
};

/**
 * @brief A bar for containment and switching of tabs.
 *
 * TabBar contains only Bars, elements are inside the Tabs created by addTab().
 */
class PF_IMGUI_EXPORT TabBar : public Element {
 public:
  /**
   * Construct TabBar.
   * @param elementName ID of the TabBar
   */
  explicit TabBar(const std::string &elementName);

  /**
   * Create a new Tab.
   * @param name ID of the Tab
   * @param caption text rendered on the Tab
   * @return reference to the newly created Tab
   */
  Tab &addTab(const std::string &name, const std::string &caption);
  /**
   * Remove a tab with the given ID.
   * If no such Tab exists nothing happens.
   * @param name ID of the tab to be removed
   */
  void removeTab(const std::string &name);

 protected:
  void renderImpl() override;

 private:
  std::vector<std::unique_ptr<Tab>> tabs;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_TABBAR_H
