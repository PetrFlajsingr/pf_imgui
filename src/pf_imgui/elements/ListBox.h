//
// Created by petr on 11/1/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_LISTBOX_H
#define PF_IMGUI_IMGUI_ELEMENTS_LISTBOX_H

#include <algorithm>
#include <pf_common/concepts/Iterable.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT ListBox : public ItemElement, public Labellable, public ValueObservable<std::string_view> {
 public:
  ListBox(const std::string &elementName, const std::string &label, std::vector<std::string> items_ = {},
          int selectedIdx = 0, int heightInItems = -1);

  void addItem(std::string item);
  void addItems(const Iterable_of<std::string> auto &data) { std::ranges::copy(data, std::back_inserter(items)); }

  [[nodiscard]] std::string_view getSelectedItem() const;

  void selectItem(std::string_view itemToSelect);

 protected:
  void renderImpl() override;

 private:
  std::vector<std::string> items;
  int currentItemIdx = 0;
  int height = -1;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_LISTBOX_H
