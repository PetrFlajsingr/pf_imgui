//
// Created by xflajs00 on 10.04.2022.
//

#include "Font.h"

namespace pf::ui::ig {

Font::Font(ImFont *value, std::string fontName) : font(value), name(fontName) {}

RAII Font::applyScoped() {
  ImGui::PushFont(font);
  return RAII{ImGui::PopFont};
}

RAII Font::applyScopedIfNotDefault() {
  if (font != nullptr) { return applyScoped(); }
  return RAII{[] {}};
}

std::string_view Font::getName() const { return name; }

bool Font::operator==(const Font &rhs) const { return font == rhs.font; }
bool Font::operator!=(const Font &rhs) const { return !(rhs == *this); }

}  // namespace pf::ui::ig