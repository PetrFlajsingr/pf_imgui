/**
 * @file Tooltip.h
 * @brief Tooltip element.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_ELEMENTS_TOOLTIP_H
#define PF_IMGUI_ELEMENTS_TOOLTIP_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Tooltip to display when hovering over elements.
 *
 * This tooltip should usually be created via createTooltip() method in ItemElement s.
 */
class PF_IMGUI_EXPORT Tooltip : public ElementWithID, public ElementContainer {
 public:
  /**
   * @brief Struct for construction of Tooltip.
   */
  struct Config {
    using Parent = Tooltip;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct Tooltip
   * @param config construction args @see Tooltip::Config
   */
  explicit Tooltip(Config &&config);
  /**
   * Construct Tooltip.
   * @param elementName ID of the tooltip
   */
  explicit Tooltip(const std::string &elementName);

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_TOOLTIP_H
