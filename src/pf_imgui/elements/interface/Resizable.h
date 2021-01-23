//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_RESIZABLEELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_RESIZABLEELEMENT_H

#include "Element.h"
#include <imgui.h>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Resizable {
 public:
  explicit Resizable(const ImVec2 &size);

  Resizable(Resizable &&other) noexcept;
  Resizable &operator=(Resizable &&other) noexcept;

  [[nodiscard]] const ImVec2 &getSize() const;
  virtual void setSize(const ImVec2 &s);

  virtual ~Resizable() = default;

 private:
  ImVec2 size;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_RESIZABLEELEMENT_H
