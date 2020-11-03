//
// Created by petr on 11/1/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_LISTBOX_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_LISTBOX_H

#include <pf_common/concepts/Iterable.h>
#include "interface/LabeledElement.h"
#include "interface/ValueObservableElement.h"
#include <vector>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT ListBox : public LabeledElement,
                     public ValueObservableElement<std::string_view> {
 public:
  ListBox(const std::string &elementName, const std::string &caption,
               std::vector<std::string> items_ = {}, int selectedIdx = 0, int heightInItems = -1);

  void addItem(std::string item);
  void addItems(const Iterable_of<std::string> auto &data) {
    std::ranges::copy(data, std::back_inserter(items));
  }

  [[nodiscard]] std::string_view getSelectedItem() const;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::string> items;
  int currentItemIdx = 0;
  int height = -1;
};
}// namespace pf::ui
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_LISTBOX_H
