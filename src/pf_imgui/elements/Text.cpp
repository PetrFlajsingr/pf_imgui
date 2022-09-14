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

Text::Text(const std::string &elementName, std::string textValue, bool wrapText)
    : ItemElement(elementName), DragSource<std::string>(false), DropTarget<std::string>(false),
      text(std::move(textValue)), wrap(wrapText) {}

void Text::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto scopedFont = font.applyScopedIfNotDefault();
  if (wrap) {
    ImGui::TextWrapped("%s", text->c_str());
  } else {
    ImGui::Text("%s", text->c_str());
  }
  drag(*text);
  if (auto drop = dropAccept(); drop.has_value()) { *text.modify() = *drop; }
}

bool Text::isWrap() const { return wrap; }

void Text::setWrap(bool textWrap) { wrap = textWrap; }

const std::string &Text::getValue() const { return *text; }

void Text::setValue(const std::string &newValue) { *text.modify() = newValue; }

Subscription Text::addValueListenerImpl(std::function<void(const std::string &)> listener) {
  return text.addListener(std::move(listener));
}

}  // namespace pf::ui::ig
