/**
 * @file Element.h
 * @brief Base class for objects which can be considered UI elements.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/fwd.h>
#include <string>
#include <experimental/memory>

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

  void render() override;
 private:
  friend class ImGuiInterface;
  ImFont* font = nullptr;
  std::experimental::observer_ptr<FontManager> fontManager;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_ELEMENT_H