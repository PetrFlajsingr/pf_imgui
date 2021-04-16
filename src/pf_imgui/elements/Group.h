//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_GROUP_H
#define PF_IMGUI_ELEMENTS_GROUP_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Collapsible.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Group of elements which may be visually delimited.
 *
 * Elements are rendered in top-down order.
 */
class PF_IMGUI_EXPORT Group : public ItemElement, public ElementContainer, public Labellable, public Collapsible {
 public:
  /**
   * Construct Group.
   * @param elementName ID of the group
   * @param label text drawn on top of the group
   * @param persistent enable state saving to disk
   * @param allowCollapse show button for collapsing the group
   */
  Group(const std::string &elementName, const std::string &label, Persistent persistent = Persistent::No,
        AllowCollapse allowCollapse = AllowCollapse::No);
  /**
   * Construct Group.
   * @param elementName ID of the group
   * @param label text drawn on top of the group
   * @param allowCollapse show button for collapsing the group
   */
  Group(const std::string &elementName, const std::string &label, AllowCollapse allowCollapse);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_GROUP_H
