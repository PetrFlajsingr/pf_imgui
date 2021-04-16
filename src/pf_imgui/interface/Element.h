//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_INTERFACE_ELEMENT_H
#define PF_IMGUI_INTERFACE_ELEMENT_H

#include "Renderable.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Interface for all ImGui elements
 *
 * Adds functionality of pushing unique names to ImGui render tree.
 *
 * @warning If there are duplicate IDs in the render tree the program may have some undefined behavior for functions which use IDs.
 * @see Renderable
 */
class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  explicit Element(const std::string &name);

  void render() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_ELEMENT_H