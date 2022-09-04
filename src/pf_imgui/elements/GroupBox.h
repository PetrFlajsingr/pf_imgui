/**
 * @file GroupBox.h
 * @brief GroupBox element.
 * @author Petr Flajšingr
 * @date 17.11.21
 */

#ifndef PF_IMGUI_ELEMENTS_GROUPBOX_H
#define PF_IMGUI_ELEMENTS_GROUPBOX_H

#include <pf_common/Explicit.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/reactive/Observable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief A group of elements with a border and a label.
 */
class PF_IMGUI_EXPORT GroupBox : public ElementWithID, public ElementContainer {
 public:
  /**
   * @brief Struct for construction of GroupBox.
   */
  struct Config {
    using Parent = GroupBox;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered on top of the group */
    Explicit<Size> size;              /*!< Size of the element */
  };
  /**
   * Construct GroupBox
   * @param config construction args @see GroupBox::Config
   */
  explicit GroupBox(Config &&config);
  /**
   * Construct GroupBox.
   * @param elementName unique name of the element
   * @param labelValue label rendered on the top of the GroupBox
   * @param s size of the GroupBox
   */
  GroupBox(const std::string &elementName, const std::string &labelValue, Size s);

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_GROUPBOX_H
