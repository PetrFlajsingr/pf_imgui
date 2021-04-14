//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_IMGUI_ELEMENTS_COMBOBOX_H

#include <optional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A typical combobox with string items.
 *
 * A combobox filled with strings. The element notifies listeners of changes in selection. It also has a filter functionality,
 * which allows for delimiting shown items without the need to rebuild them.
 *
 * @warning If there are multiple items which are the same some unexpected behavior may occur.
 *
 * @todo: built-in filter
 * @todo: generic item
 */
class PF_IMGUI_EXPORT ComboBox : public ItemElement,
                                 public Labellable,
                                 public ValueObservable<std::string_view>,
                                 public Savable {
 public:
  /**
   * Construct Combobox.
   * @param elementName ID of the combobox
   * @param label label drawn next to the element
   * @param previewValue value shown when no item is selected
   * @param items
   * @param persistent
   */
  ComboBox(const std::string &elementName, const std::string &label, std::string previewValue,
           std::vector<std::string> items, Persistent persistent = Persistent::No);

  /**
   * Get currently selected item.
   * @return if any item is selected return it, otherwise std::nullopt
   */
  [[nodiscard]] std::optional<std::string_view> getSelectedItem();
  /**
   * Set selected item. If no such item is found the selection is cancelled.
   * @param item item to be selected
   */
  void setSelectedItem(const std::string &item);
  /**
   * Cancel selection and show previewValue.
   */
  void cancelSelection();
  /**
   * Remove this item from the Combobox. If no such item exists nothing happens.
   * @param item item to be removed
   */
  void removeItem(const std::string &item);
  /**
   * Add a new item to the end of the items.
   * @param item item to be added
   */
  void addItem(const std::string &item);

  /**
   * Get all items.
   * @return items in the combobox
   */
  [[nodiscard]] const std::vector<std::string> &getItems() const;
  /**
   * Set new items, overwriting the old ones.
   * @param newItems items to set
   */
  void setItems(std::vector<std::string> newItems);

  /**
   * Set a predicate that filters which items are displayed to the user.
   * @param filterFnc predicate returning true for items which should be shown to the user
   */
  void setFilter(std::predicate<std::string_view> auto filterFnc) { filter = filterFnc; }
  /**
   * Remove item filter.
   */
  void clearFilter();

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;
  void renderImpl() override;

 private:
  std::vector<std::string> items;
  std::string previewValue;
  std::optional<unsigned int> selectedItemIndex = std::nullopt;
  std::function<bool(std::string_view)> filter = [](auto) { return true; };
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_COMBOBOX_H
