/**
 * @file Position.h
 * @brief Type used for position in pf_imgui.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_IMGUI_POSITION_H
#define PF_IMGUI_POSITION_H

#include <compare>
#include <imgui.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

/**
 * @brief Position relative to left top corner of some area.
 */
struct PF_IMGUI_EXPORT Position {
  constexpr Position() = default;
  constexpr Position(float initialX, float initialY) : x(initialX), y(initialY) {}
  constexpr explicit Position(ImVec2 pos) : x(pos.x), y(pos.y) {}

  [[nodiscard]] static Position LeftTop() { return {0.f, 0.f}; }

  /**
   * Create a position moved by given parameters.
   * @param deltaX difference on X axis
   * @param deltaY difference on Y axis
   * @return calculated Position
   */
  [[nodiscard]] constexpr Position moveDelta(float deltaX, float deltaY) const { return {x + deltaX, y + deltaY}; }

  float x{};
  float y{};

  constexpr auto operator<=>(const Position &) const = default;

  constexpr explicit operator ImVec2() const { return {x, y}; }
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_POSITION_H
