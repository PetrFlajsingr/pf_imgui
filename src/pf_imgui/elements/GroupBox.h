/**
 * @file GroupBox.h
 * @brief GroupBox element.
 * @author Petr Flajšingr
 * @date 17.11.21
 */

#ifndef PF_IMGUI_GROUPBOX_H
#define PF_IMGUI_GROUPBOX_H

#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

/**
 * @brief A group of elements with a border and a label.
 */
class GroupBox : public Element, public ElementContainer, public Labellable, public Resizable {
 public:
  /**
   * Construct GroupBox.
   * @param name unique name of the element
   * @param label label rendered on the top of the groupbox
   * @param s size of the groupbox
   */
  GroupBox(const std::string &name, const std::string &label, const Size &s);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_GROUPBOX_H
