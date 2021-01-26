//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H

#include "ResizableLayout.h"
#include <algorithm>
#include <concepts>
#include <memory>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <vector>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT GridLayout : public ResizableLayout {
 public:
  GridLayout(const std::string &elementName, const ImVec2 &size, uint32_t width, uint32_t height,
             bool showBorder = false);

  void setLayoutForCell(uint32_t column, uint32_t row, std::unique_ptr<ResizableLayout> layout);

  [[nodiscard]] ResizableLayout &getCellLayout(uint32_t column, uint32_t row);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  uint32_t width;
  uint32_t height;
  std::vector<std::unique_ptr<ResizableLayout>> cells;

  [[nodiscard]] uint32_t indexForCell(uint32_t column, uint32_t row) const;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_GRIDLAYOUT_H
