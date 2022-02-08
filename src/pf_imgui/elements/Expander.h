/**
 * @file Group.h
 * @brief Group element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

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
 * @brief Group of elements which is visually delimited with a header.
 *
 * Elements are rendered in top-down order.
 */
class PF_IMGUI_EXPORT Expander : public ItemElement, public ElementContainer, public Labellable, public Collapsible {
 public:
  /**
   * Construct Expander.
   * @param elementName ID of the group
   * @param label text drawn on top of the Expander
   * @param persistent enable state saving to disk
   * @param allowCollapse show button for collapsing the Expander
   */
  Expander(const std::string &elementName, std::unique_ptr<Resource<std::string>> label,
           Persistent persistent = Persistent::No, AllowCollapse allowCollapse = AllowCollapse::Yes);
  /**
   * Construct Group.
   * @param elementName ID of the group
   * @param label text drawn on top of the group
   * @param allowCollapse show button for collapsing the group
   */
  Expander(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, AllowCollapse allowCollapse);

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_GROUP_H
