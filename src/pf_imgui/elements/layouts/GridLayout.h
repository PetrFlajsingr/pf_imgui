//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H

#include "Layout.h"
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <vector>

// TODO: implement
namespace pf::ui::ig {
class PF_IMGUI_EXPORT GridLayout : public Layout {
 public:

 private:
  uint32_t width;
  uint32_t height;
};
}
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H
