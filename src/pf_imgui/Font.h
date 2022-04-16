//
// Created by xflajs00 on 10.04.2022.
//

#ifndef IMGUI_EXPERIMENTS_FONT_H
#define IMGUI_EXPERIMENTS_FONT_H

#include <imgui.h>
#include <pf_common/RAII.h>
#include <string>
#include <string_view>

namespace pf::ui::ig {
class FontManager;

/**
 * @brief Abstraction of fonts for Elements.
 */
class Font {
 private:
  friend class FontManager;
  explicit Font(ImFont *value, std::string fontName);

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

  [[nodiscard]] static inline Font Default() { return Font{nullptr, "default"}; }

 private:
  ImFont *font;
  std::string name;
};
}  // namespace pf::ui::ig

#endif  //IMGUI_EXPERIMENTS_FONT_H
