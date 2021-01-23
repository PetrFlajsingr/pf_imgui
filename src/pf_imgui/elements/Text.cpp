//
// Created by petr on 10/31/20.
//

#include "Text.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Text::Text(const std::string &elementName, std::string text)
    : ItemElement(elementName), text(std::move(text)), color(std::nullopt) {}

Text::Text(const std::string &elementName, std::string text, ImVec4 textColor)
    : ItemElement(elementName), text(std::move(text)), color(textColor) {}

const std::string &Text::getText() const { return text; }

void Text::setText(const std::string &txt) { text = txt; }

void Text::renderImpl() {
  if (color.has_value()) {
    ImGui::TextColored(*color, "%s", text.c_str());
  } else {
    ImGui::Text("%s", text.c_str());
  }
}

}// namespace pf::ui::ig