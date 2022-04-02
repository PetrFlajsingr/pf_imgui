//
// Created by petr on 10/31/20.
//

#include "Text.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Text::Text(Text::Config &&config)
    : ItemElement(std::string{config.name}), DragSource<std::string>(false), DropTarget<std::string>(false),
      text(std::move(config.text)) {}

Text::Text(const std::string &elementName, std::string text)
    : ItemElement(elementName), DragSource<std::string>(false), DropTarget<std::string>(false), text(std::move(text)) {}

std::string_view Text::getText() const { return text; }

void Text::renderImpl() {
  auto colorStyle = setColorStack();
  ImGui::Text("%s", text.c_str());
  drag(text);
  if (auto drop = dropAccept(); drop.has_value()) { setText(*drop); }
}

void Text::setTextInner(std::string txt) { text = std::move(txt); }

}  // namespace pf::ui::ig
