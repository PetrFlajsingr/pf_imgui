//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H

#include "Element.h"
#include "../Tooltip.h"
#include <memory>

namespace pf::ui::ig {

class ItemElement : public virtual Element {
 public:
  explicit ItemElement(const std::string &elementName);

  void render() override;

  [[nodiscard]] bool isFocused() const;
  [[nodiscard]] bool isHovered() const;

  [[nodiscard]] bool hasTooltip() const;
  void removeTooltip();
  void setTooltip(std::string_view text);
  [[nodiscard]] Tooltip &createTooltip();

 private:
  bool focused = false;
  bool hovered = false;
  std::unique_ptr<Tooltip> tooltip = nullptr;
};

}// namespace pf
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
