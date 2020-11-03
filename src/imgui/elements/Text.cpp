//
// Created by petr on 10/31/20.
//

#include "Text.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Text::Text(const std::string &elementName, std::string text)
    : Element(elementName), text(std::move(text)) {}

const std::string &Text::getText() const { return text; }
void Text::setText(const std::string &txt) { text = txt; }

void Text::renderImpl() { ImGui::Text("%s", text.c_str()); }
}// namespace pf::ui