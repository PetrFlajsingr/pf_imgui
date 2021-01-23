//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_COMBOBOX_H
#define PF_IMGUI_IMGUI_ELEMENTS_COMBOBOX_H

#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/Savable.h"
#include "interface/ValueObservable.h"
#include <optional>
#include <pf_imgui/_export.h>
#include <string>
#include <vector>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT ComboBox : public ItemElement,
                                 public Labellable,
                                 public ValueObservable<std::string_view>,
                                 public Savable {
 public:
  ComboBox(const std::string &elementName, const std::string &label, std::string previewValue,
           std::vector<std::string> items, Persistent persistent = Persistent::No);

  [[nodiscard]] std::optional<std::string_view> getSelectedItem();
  void removeItem(const std::string &item);
  void addItem(const std::string &item);

  [[nodiscard]] const std::vector<std::string> &getItems() const;
  void setItems(std::vector<std::string> newItems);

  void setFilter(std::predicate<std::string_view> auto filterFnc) { filter = filterFnc; }
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
