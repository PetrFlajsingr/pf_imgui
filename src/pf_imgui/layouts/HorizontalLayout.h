/**
 * @file HorizontalLayout.h
 * @brief Layout rendering its children horizontally.
 * @author Petr Flajšingr
 * @date 10.4.22
 */

#ifndef IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H
#define IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H

#include "LinearLayout.h"
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
    std::string_view name;                  /*!< Unique name of the element */
    Size size;                              /*!< Size of the element */
    float spacing = 0.f;                    /*!< Spaces between elements */
    ShowBorder showBorder = ShowBorder::No; /*!< Render border around layout's area */
  };
  /**
   * Config constructor of HorizontalLayout @see HorizontalLayout::Config
   * @param config construction args
   */
  explicit HorizontalLayout(Config &&config);

  /**
   * Construct HorizontalLayout
   * @param name unique name of the element
   * @param size size of the layout
   * @param elementSpacing spaces between elements
   * @param showBorder render border around layout's area
   */
  HorizontalLayout(const std::string &name, Size size, float elementSpacing = 0.f,
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

#endif  //IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H
