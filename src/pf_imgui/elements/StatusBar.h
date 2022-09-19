//
// Created by xflajs00 on 15.11.2021.
//

#ifndef PF_IMGUI_ELEMENTS_STATUSBAR_H
#define PF_IMGUI_ELEMENTS_STATUSBAR_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ElementWithID.h>

namespace pf::ui::ig {

/**
 * @brief A status bar rendered on the bottom of the main window.
 */
class PF_IMGUI_EXPORT AppStatusBar : public ElementWithID, public ElementContainer {
 public:
  /**
   * @brief Struct for construction of AppStatusBar.
   */
  struct Config {
    using Parent = AppStatusBar;
    Explicit<std::string_view> name; /*!< Unique name of the element */
  };
  /**
   * Construct AppStatusBar
   * @param config construction args @see AppStatusBar::Config
   */
  explicit AppStatusBar(Config &&config);
  /**
   * Construct AppStatusBar.
   * @param elementName unique name of the element
   */
  explicit AppStatusBar(std::string_view elementName);

  [[nodiscard]] float getHeight() const;

 protected:
  void renderImpl() override;

 private:
  float height = 0.f;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_ELEMENTS_STATUSBAR_H
