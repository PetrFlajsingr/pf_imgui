/**
 * @file VerticalLayout.h
 * @brief Layout rendering its children vertically.
 * @author Petr Flajšingr
 * @date 10.4.22
 */

#ifndef IMGUI_EXPERIMENTS_VERTICALLAYOUT_H
#define IMGUI_EXPERIMENTS_VERTICALLAYOUT_H

#include "LinearLayout.h"

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
    std::string_view name;                  /*!< Unique name of the element */
    Size size;                              /*!< Size of the element */
    float spacing = 0.f;                    /*!< Spaces between elements */
    ShowBorder showBorder = ShowBorder::No; /*!< Render border around layout's area */
  };
  /**
   * Config constructor of VerticalLayout @see VerticalLayout::Config
   * @param config construction args
   */
  explicit VerticalLayout(Config &&config);

  /**
   * Construct HorizontalLayout
   * @param name unique name of the element
   * @param size size of the layout
   * @param elementSpacing spaces between elements
   * @param showBorder render border around layout's area
   */
  VerticalLayout(const std::string &name, Size size, float elementSpacing = 0.f,
                 ShowBorder showBorder = ShowBorder::No);

  /**
   *
   * @return current spacing between elements
   */
  [[nodiscard]] float getSpacing() const;
  /**
   * Set spacing between elements
   * @param newSpacing
   */
  void setSpacing(float newSpacing);

 protected:
  void renderImpl() override;

 private:
  float spacing;
};

}  // namespace pf::ui::ig
#endif  //IMGUI_EXPERIMENTS_VERTICALLAYOUT_H
