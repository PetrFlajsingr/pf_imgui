//
// Created by petr on 6/15/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/Renderable.h>
#include <vector>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT RenderablesContainer : public AllColorCustomizable, public AllStyleCustomizable {
 public:
  /**
   * Get all renderables stored inside the container.
   * @return vector of pointers to renderables
   */
  [[nodiscard]] virtual std::vector<Renderable *> getRenderables() = 0;
  virtual ~RenderablesContainer() = default;

 protected:
  using AllColorCustomizable::setColorStack;
  using AllStyleCustomizable::setStyleStack;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
