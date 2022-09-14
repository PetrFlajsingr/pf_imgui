/**
 * @file VerticalLayout.h
 * @brief Layout rendering its children vertically.
 * @author Petr Flajšingr
 * @date 10.4.22
 */

#ifndef IMGUI_EXPERIMENTS_VERTICALLAYOUT_H
#define IMGUI_EXPERIMENTS_VERTICALLAYOUT_H

#include "LinearLayout.h"
#include <pf_common/Explicit.h>

namespace pf::ui::ig {

// TODO: merge with HorizontalLayout? or make base class
/**
 * @brief Layout rendering its children vertically with optional spacing.
 */
class VerticalLayout : public LinearLayout {
 public:
  /**
   * @brief Struct for construction of VerticalLayout.
   */
  struct Config {
    using Parent = VerticalLayout;
    Explicit<std::string_view> name;           /*!< Unique name of the element */
    Explicit<Size> size;                       /*!< Size of the element */
    VerticalAlign align = VerticalAlign::Left; /*!< ElementWithID alignment */
    bool showBorder = false;                   /*!< Render border around layout's area */
  };
  /**
   * Config constructor of VerticalLayout @see VerticalLayout::Config
   * @param config construction args
   */
  explicit VerticalLayout(Config &&config);

  /**
   * Construct HorizontalLayout
   * @param elementName unique name of the element
   * @param initialSize size of the layout
   * @param align element alignment
   * @param showBorder render border around layout's area
   */
  VerticalLayout(std::string_view elementName, Size initialSize, VerticalAlign align = VerticalAlign::Left,
                 ShowBorder showBorder = ShowBorder::No);

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] float alignmentAsFloat() const;

  VerticalAlign alignment;
};

}  // namespace pf::ui::ig
#endif  //IMGUI_EXPERIMENTS_VERTICALLAYOUT_H
