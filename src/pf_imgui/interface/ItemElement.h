//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H

#include "Element.h"
#include "Focusable.h"
#include "Hoverable.h"
#include "src/pf_imgui/elements/Tooltip.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT ItemElement : public Element, public Focusable, public Hoverable {
 public:
  explicit ItemElement(const std::string &elementName);

  ItemElement(ItemElement &&other) noexcept;
  ItemElement &operator=(ItemElement &&other) noexcept;

  void render() override;

  [[nodiscard]] bool hasTooltip() const;
  void removeTooltip();
  void setTooltip(std::string_view text);
  [[nodiscard]] Tooltip &createTooltip();

  void setFocus() override;

 private:
  std::unique_ptr<Tooltip> tooltip = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
