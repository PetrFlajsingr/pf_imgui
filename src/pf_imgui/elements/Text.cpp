//
// Created by petr on 10/31/20.
//

#include "Text.h"
#include <imgui.h>
#include <utility>

namespace pf::ui::ig {

Text::Text(Text::Config &&config)
    : ItemElement(std::string{config.name.value}), DragSource<std::string>(false), DropTarget<std::string>(false),
      text(std::move(config.text)), wrap(config.wrapText) {}

Text::Text(const std::string &elementName, std::string text, bool wrapText)
    : ItemElement(elementName), DragSource<std::string>(false), DropTarget<std::string>(false), text(std::move(text)),
      wrap(wrapText) {}

std::string_view Text::getText() const { return text; }

void Text::setText(std::string newText) { setTextInner(std::move(newText)); }

void Text::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto scopedFont = applyFont();
  if (wrap) {
    ImGui::TextWrapped("%s", text.c_str());
  } else {
    ImGui::Text("%s", text.c_str());
  }
  drag(text);
  if (auto drop = dropAccept(); drop.has_value()) { setText(*drop); }
}

void Text::setTextInner(std::string txt) { text = std::move(txt); }

bool Text::isWrap() const { return wrap; }

void Text::setWrap(bool textWrap) { wrap = textWrap; }

}  // namespace pf::ui::ig
