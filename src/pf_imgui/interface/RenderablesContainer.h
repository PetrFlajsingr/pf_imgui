//
// Created by petr on 6/15/21.
//

#ifndef PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
#define PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H

#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <vector>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT RenderablesContainer {
 public:
  /**
   * Get all renderables stored inside the container.
   * @return vector of pointers to renderables
   */
  [[nodiscard]] virtual std::vector<Renderable *> getRenderables() = 0;
  virtual ~RenderablesContainer() = default;

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_RENDERABLESCONTAINER_H
