//
// Created by petr on 1/24/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STACKEDLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STACKEDLAYOUT_H

#include "ResizableLayout.h"
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/interface/Container.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT StackedLayout : public ResizableLayout {
  struct StackContainer : public Container {};

 public:
  StackedLayout(const std::string &elementName, const ImVec2 &size, bool showBorder = false);
  StackContainer &pushStack();
  void removeStack(std::size_t index);
  void moveStack(std::size_t srcIndex, std::size_t dstIndex);

  [[nodiscard]] std::size_t getCurrentIndex() const;
  void setIndex(std::size_t index);

  [[nodiscard]] StackContainer &getCurrentStack();
  [[nodiscard]] StackContainer &getStackAtIndex(std::size_t index);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::optional<std::size_t> selectedIndex = std::nullopt;
  std::vector<StackContainer> stacks;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STACKEDLAYOUT_H
