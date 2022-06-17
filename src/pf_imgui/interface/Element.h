/**
 * @file Element.h
 * @brief Base class for objects which can be considered UI elements and need ID or/and a font.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include <pf_imgui/Font.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/interface/ElementBase.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Interface for all ImGui elements with IDs
 *
 * Adds functionality of pushing unique names to ImGui render tree.
 *
 * @warning If there are duplicate IDs in the render tree the program may have some undefined behavior for functions which use IDs.
 * @see Renderable
 *
 * @attention For classes which don't need IDs use ElementBaase
 * @attention For classes with hover/focus/tooltip etc. use ItemElement
 */
class PF_IMGUI_EXPORT Element : public ElementBase {
 public:
  /**
   * Construct Element.
   * @param name ID of the element.
   */
  explicit Element(const std::string &name);

  /**
   * Only available after first render call.
   * @return
   */
  [[nodiscard]] ImGuiID getId() const;

  void render() override;
  /**
   * Set font for Element and all elements inside - except for those that have their own font.
   * @param font new font
   */
  void setFont(Font newFont);
  [[nodiscard]] Font getFont() const;

 protected:
  void setId(ImGuiID newId);
  Font font = Font::Default();

 private:
  ImGuiID id;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_ELEMENT_H
