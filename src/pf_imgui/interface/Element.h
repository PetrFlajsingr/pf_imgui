/**
 * @file Element.h
 * @brief Base class for objects which can be considered UI elements.
 * @author Petr Flajšingr
 * @date 17.6.22
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Event.h>

namespace pf::ui::ig {

/**
 * @brief Base class for anythin which can be considered a UI element.
 *
 * @attention For classes needing ids/fonts use ElementWithID
 * @attention For classes with hover/focus/tooltip etc. use ItemElement
 */
class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  /**
   * Construct Element.
   * @param elementName ID of the element.
   */
  explicit Element(const std::string &elementName);
  ~Element() override;
  Element(Element &&other) noexcept;
  Element &operator=(Element &&other) noexcept;


  /**
   * Called during object's destruction. The object will be partially destroyed at this point.
   */
  ClassEvent<Element> destroyEvent;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_INTERFACE_ELEMENT_H
