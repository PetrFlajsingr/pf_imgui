/**
 * @file Element.h
 * @brief Base class for objects which can be considered UI elements.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include <pf_imgui/Font.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <pf_imgui/interface/Renderable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Interface for all ImGui elements
 *
 * Adds functionality of pushing unique names to ImGui render tree.
 *
 * @warning If there are duplicate IDs in the render tree the program may have some undefined behavior for functions which use IDs.
 * @see Renderable
 *
 */
class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  /**
   * Construct Element.
   * @param name ID of the element.
   */
  explicit Element(const std::string &name);
  ~Element() override;
  Element(Element &&other) noexcept;
  Element &operator=(Element &&other) noexcept;

  void render() override;
  /**
   * Set font for Element and all elements inside - except for those that have their own font.
   * @param font new font
   */
  void setFont(Font newFont);
  [[nodiscard]] Font getFont() const;

  /**
   * Only available after first render call.
   * @return
   */
  [[nodiscard]] ImGuiID getId() const;

  /**
   * Add a listener called when this object is destroyed.
   * Most of the object will be destroyed at this point.
   *
   * @param listener listener to call
   * @return Subscription to allow for listener cancellation
   */
  Subscription addDestroyListener(std::invocable auto &&listener) {
    return observableDestroy.addListener(std::forward<decltype(listener)>(listener));
  }

 protected:
  Font font = Font::Default();

 private:
  Observable_impl<> observableDestroy;
  ImGuiID id;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_ELEMENT_H
