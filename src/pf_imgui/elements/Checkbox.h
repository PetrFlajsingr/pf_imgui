//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_CHECKBOX_H
#define PF_IMGUI_ELEMENTS_CHECKBOX_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Checkbox : public ItemElement, public ValueObservable<bool>, public Labellable, public Savable {
 public:
  enum class Type {
    Checkbox, Toggle
  };
  Checkbox(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
           bool value = false);
  Checkbox(const std::string &elementName, const std::string &label, Type checkboxType, Persistent persistent = Persistent::No,
           bool value = false);

  void setSelected(bool value);
  [[nodiscard]] bool isSelected() const;
  void toggle();

 protected:
  Type type;
  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_CHECKBOX_H
