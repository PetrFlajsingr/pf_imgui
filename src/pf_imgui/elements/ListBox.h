/**
 * @file ListBox.h
 * @brief ListBox element.
 * @author Petr Flajšingr
 * @date 1.11.20
 */


#ifndef PF_IMGUI_ELEMENTS_LISTBOX_H
#define PF_IMGUI_ELEMENTS_LISTBOX_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {
/**
 * @brief Container for strings shown as list box and selectable by user.
 *
 * User selection can be observed via listeners.
 *
 * @todo: generic?
 */
class PF_IMGUI_EXPORT ListBox : public ItemElement, public Labellable, public ValueObservable<std::string_view> {
 public:
  /**
   * Construct ListBox.
   * @param elementName ID of the element
   * @param label text rendered at the top of the list
   * @param items_ starting items in the list
   * @param selectedIdx starting selected id
   * @param heightInItems items to show before scroll is enabled - -1 shows all
   */
  ListBox(const std::string &elementName, const std::string &label, std::vector<std::string> items_ = {},
          int selectedIdx = 0, int heightInItems = -1);

  /**
   * Add item to the end of the list.
   * @param item item to be added
   */
  void addItem(std::string item);
  /**
   * Add items to the end of the list.
   * @param data items to be added
   */
  void addItems(const std::ranges::range auto &data) requires(
      std::same_as<std::ranges::range_value_t<decltype(data)>, std::string>) {
    std::ranges::copy(data, std::back_inserter(items));
  }
  /**
   * Overwrite current items with the ones provided.
   * @param data new items
   */
  void setItems(const std::ranges::range auto &data) requires(
      std::same_as<std::ranges::range_value_t<decltype(data)>, std::string>) {
    items.clear();
    currentItemIdx = 0;
    std::ranges::copy(data, std::back_inserter(items));
  }

  /**
   * Get and item currently selected by the user.
   * @param data
   */
  [[nodiscard]] std::string_view getSelectedItem() const;

  /**
   * Set selected item by name. If no such item is found nothing happens.
   * @param itemToSelect item to select
   */
  void selectItem(std::string_view itemToSelect);

 protected:
  void renderImpl() override;

 private:
  std::vector<std::string> items;
  int currentItemIdx = 0;
  int height = -1;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_LISTBOX_H
