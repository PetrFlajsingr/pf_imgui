//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_RENDERABLE_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_RENDERABLE_H

#include <pf_common/enums.h>
#include <pf_imgui/enums.h>

namespace pf::ui::ig {

class Renderable {
 public:
  Renderable() = default;
  Renderable(Renderable &&other) noexcept;
  Renderable &operator=(Renderable &&other) noexcept;
  Renderable(const Renderable &) = delete;
  Renderable &operator=(const Renderable &) = delete;

  /**
   * DO NOT OVERRIDE UNLESS YOU WANT TO LOSE SOME FUNCTIONALITY
   */
  virtual void render();

  virtual ~Renderable() = default;

  [[nodiscard]] Visibility getVisibility() const;
  void setVisibility(Visibility visibility);

  [[nodiscard]] Enabled getEnabled() const;
  void setEnabled(Enabled eleState);

 protected:
  virtual void renderImpl() = 0;

 private:
  Visibility visibility = Visibility::Visible;
  Enabled enabled = Enabled::Yes;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_RENDERABLE_H
