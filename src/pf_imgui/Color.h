//
// Created by xflajs00 on 02.04.2022.
//

#ifndef PF_IMGUI_COLOR_H
#define PF_IMGUI_COLOR_H

#include <imgui.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Color {
 public:
  Color() : color(IM_COL32(255, 255, 255, 255)) {}

 private:
  ImColor color;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COLOR_H
