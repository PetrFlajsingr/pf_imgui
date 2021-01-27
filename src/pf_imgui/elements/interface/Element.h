//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H
#define PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H

#include "Renderable.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  explicit Element(const std::string &name);
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H