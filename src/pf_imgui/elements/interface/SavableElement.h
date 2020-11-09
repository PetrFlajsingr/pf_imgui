//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_SAVABLEELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_SAVABLEELEMENT_H

#include "Element.h"
#include <pf_imgui/_export.h>
#include <toml++/toml_table.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT SavableElement : public virtual Element {
 public:
  SavableElement(const std::string &elementName, Persistent persistent);

  SavableElement(SavableElement &&other) noexcept;
  SavableElement &operator=(SavableElement &&other) noexcept;

  void unserialize(const toml::table &src);
  std::optional<toml::table> serialize();

 protected:
  virtual void unserialize_impl(const toml::table &src) = 0;
  virtual toml::table serialize_impl() = 0;

 private:
  bool persist;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_SAVABLEELEMENT_H
