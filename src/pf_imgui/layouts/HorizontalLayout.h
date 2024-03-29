/**
 * @file HorizontalLayout.h
 * @brief Layout rendering its children horizontally.
 * @author Petr Flajšingr
 * @date 10.4.22
 */

#ifndef PF_IMGUI_LAYOUTS_HORIZONTALLAYOUT_H
#define PF_IMGUI_LAYOUTS_HORIZONTALLAYOUT_H

#include "LinearLayout.h"
#include <pf_common/Explicit.h>

namespace pf::ui::ig {

/**
 * @brief Layout rendering its children horizontal with optional spacing.
 */
class PF_IMGUI_EXPORT HorizontalLayout : public LinearLayout {
 public:
  /**
   * @brief Struct for construction of HorizontalLayout.
   */
  struct Config {
    using Parent = HorizontalLayout;
    Explicit<std::string_view> name;             /*!< Unique name of the element */
    Explicit<Size> size;                         /*!< Size of the element */
    HorizontalAlign align = HorizontalAlign::Up; /*!< ElementWithID alignment */
    bool showBorder = false;                     /*!< Render border around layout's area */
  };
  /**
   * Config constructor of HorizontalLayout @see HorizontalLayout::Config
   * @param config construction args
   */
  explicit HorizontalLayout(Config &&config);

  /**
   * Construct HorizontalLayout
   * @param elementName unique name of the element
   * @param initialSize size of the layout
   * @param align element alignment
   * @param showBorder render border around layout's area
   */
  HorizontalLayout(std::string_view elementName, Size initialSize, HorizontalAlign align = HorizontalAlign::Up,
                   ShowBorder showBorder = ShowBorder::No);

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] float alignmentAsFloat() const;

  HorizontalAlign alignment;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_LAYOUTS_HORIZONTALLAYOUT_H
