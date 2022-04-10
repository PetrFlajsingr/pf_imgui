//
// Created by xflajs00 on 10.04.2022.
//

#ifndef IMGUI_EXPERIMENTS_FONT_H
#define IMGUI_EXPERIMENTS_FONT_H

#include <pf_common/RAII.h>
#include <imgui.h>
#include <string_view>

namespace pf::ui::ig {
class FontManager;

/**
 * @brief Abstraction of fonts for Elements.
 */
class Font {
 private:
  friend class FontManager;
  explicit Font(ImFont *value);

 public:
  Font(const Font &other) = default;
  Font &operator=(const Font &other) = default;
  Font(Font &&other) = default;
  Font &operator=(Font &&other) = default;

  [[nodiscard]] RAII applyScoped();
  [[nodiscard]] RAII applyScopedIfNotDefault();

  [[nodiscard]] bool operator==(const Font &rhs) const;
  [[nodiscard]] bool operator!=(const Font &rhs) const;

  [[nodiscard]] std::string_view getName() const;

  static inline Font Default() { return Font{nullptr}; }

 private:
  ImFont *font;
};
}

#endif  //IMGUI_EXPERIMENTS_FONT_H
