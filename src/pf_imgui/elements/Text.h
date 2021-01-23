//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_TEXT_H
#define PF_IMGUI_IMGUI_ELEMENTS_TEXT_H

#include "interface/Element.h"
#include "interface/ItemElement.h"
#include <imgui.h>
#include <optional>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Text : public ItemElement {
 public:
  Text(const std::string &elementName, std::string text);
  Text(const std::string &elementName, std::string text, ImVec4 textColor);

  [[nodiscard]] const std::string &getText() const;
  void setText(const std::string &text);

 protected:
  void renderImpl() override;

 private:
  std::string text;
  std::optional<ImVec4> color;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_TEXT_H
