//
// Created by petr on 10/31/20.
//

#ifndef REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_COMBOBOX_H
#define REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_COMBOBOX_H

#include "interface/LabeledElement.h"
#include "interface/SavableElement.h"
#include "interface/ValueObservableElement.h"
#include <optional>
#include <vector>

namespace pf::ui::ig {
class ComboBox : public LabeledElement,
                 public ValueObservableElement<std::string_view>,
                 public SavableElement {
 public:
  ComboBox(const std::string &elementName, const std::string &caption, std::string previewValue,
           std::vector<std::string> items, Persistent persistent = Persistent::No);

  [[nodiscard]] std::optional<std::string_view> getSelectedItem();
  void removeItem(const std::string &item);
  void addItem(const std::string &item);

 protected:
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;
  void renderImpl() override;

 private:
  std::vector<std::string> items;
  std::string previewValue;
  std::optional<uint> selectedItemIndex = std::nullopt;
};
}// namespace pf::ui::ig
#endif//REALISTIC_VOXEL_RENDERING_UI_IMGUI_ELEMENTS_COMBOBOX_H
