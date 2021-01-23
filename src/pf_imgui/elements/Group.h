//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_GROUP_H
#define PF_IMGUI_IMGUI_ELEMENTS_GROUP_H

#include "interface/Collapsible.h"
#include "interface/Container.h"
#include "interface/Labellable.h"
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Savable.h>
#include <string>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Group : public Element, public Container, public Labellable, public Savable, public Collapsible {
 public:
  Group(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse = AllowCollapse::No,
        Persistent persistent = Persistent::No);

 protected:
  void renderImpl() override;
  void collapse_impl(bool) override;

  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

 private:
  AllowCollapse collapsible;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_GROUP_H
