/**
 * @file Splitter.h
 * @brief Visually split UI.
 * @author Petr Flajšingr
 * @date 15.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_SPLITTER_H
#define PF_IMGUI_ELEMENTS_SPLITTER_H

#include <pf_imgui/interface/Element.h>

namespace pf::ui::ig {

/**
 * @brief Visual separation of UI.
 *
 * Automatically detects if it should be horizontal or vertical.
 */
class Separator : public Element {
 public:
  /**
   * Construct Separator.
   * @param name element ID
   */
  explicit Separator(const std::string &name);

 protected:
  void renderImpl() override;
};

}

#endif//PF_IMGUI_ELEMENTS_SPLITTER_H
