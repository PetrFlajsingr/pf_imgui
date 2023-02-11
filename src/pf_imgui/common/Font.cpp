//
// Created by xflajs00 on 10.04.2022.
//

#include "Font.h"

namespace pf::ui::ig {

Font::Font(ImFont *value, std::string fontName) : font(value), name(std::move(fontName)) {}

std::string_view Font::getName() const { return name; }

ScopeExit<std::function<void()>> Font::applyScoped() {
  ImGui::PushFont(font);
  return ScopeExit<std::function<void()>>{ImGui::PopFont};
}
ScopeExit<std::function<void()>> Font::applyScopedIfNotDefault() {
  if (font != nullptr) { return applyScoped(); }
  return ScopeExit<std::function<void()>>{[] {}};
}

bool Font::operator==(const Font &rhs) const { return font == rhs.font; }
bool Font::operator!=(const Font &rhs) const { return !(rhs == *this); }

}  // namespace pf::ui::ig