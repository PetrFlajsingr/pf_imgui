//
// Created by petr on 1/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H

#include "../Tooltip.h"
#include "Element.h"
#include "Focusable.h"
#include "Hoverable.h"
#include <memory>
#include <string>

namespace pf::ui::ig {

// TODO: observable focus & hovered
class ItemElement : public virtual Element, public Focusable, public Hoverable {
 public:
  explicit ItemElement(const std::string &elementName);

  ItemElement(ItemElement &&other) noexcept;
  ItemElement &operator=(ItemElement &&other) noexcept;

  void render() override;

  [[nodiscard]] bool hasTooltip() const;
  void removeTooltip();
  void setTooltip(std::string_view text);
  [[nodiscard]] Tooltip &createTooltip();

 protected:
  void setFocus_impl() override;

 private:
  std::unique_ptr<Tooltip> tooltip = nullptr;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_INTERFACE_ITEMELEMENT_H
