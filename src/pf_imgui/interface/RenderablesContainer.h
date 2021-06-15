//
// Created by petr on 6/15/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H

#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {
class RenderablesContainer {
 public:
  /**
   * Get all renderables stored inside the container.
   * @return vector of pointers to renderables
   */
  virtual std::vector<Renderable *> getRenderables() = 0;
  virtual ~RenderablesContainer() = default;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
