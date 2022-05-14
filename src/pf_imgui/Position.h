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
  Position() = default;
  Position(float x, float y);
  explicit Position(ImVec2 pos);

  [[nodiscard]] static Position LeftTop();

  /**
   * Create a position moved by given parameters.
   * @param deltaX difference on X axis
   * @param deltaY difference on Y axis
   * @return calculated Position
   */
  [[nodiscard]] Position moveDelta(float deltaX, float deltaY) const;

  float x;
  float y;

  auto operator<=>(const Position &) const = default;

  explicit operator ImVec2() const;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_POSITION_H
