/**
 * @file Positionable.h
 * @brief Interface for elements which can be positioned.
 * @author Petr Flajšingr
 * @date 25.1.21
 */

#ifndef PF_IMGUI_INTERFACE_POSITIONABLE_H
#define PF_IMGUI_INTERFACE_POSITIONABLE_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <utility>

namespace pf::ui::ig {

struct PF_IMGUI_EXPORT Position {
  Position() = default;
  Position(float x, float y);
  explicit Position(ImVec2 pos);

  [[nodiscard]] static Position LeftTop();

  [[nodiscard]] Position moveDelta(float deltaX, float deltaY) const;

  float x;
  float y;

  auto operator<=>(const Position &) const = default;

  [[nodiscard]] ImVec2 asImVec() const;
};

/**
 * @brief Interface for positionable elements
 *
 * Provides a functionality to manage and observe position for the subclass.
 */
class PF_IMGUI_EXPORT Positionable {
 public:
  /**
   * Construct Positionable with starting position.
   * @param position starting position
   */
  explicit Positionable(const Position &position);

  /**
   * Add a listener, which is called every time the elements' position is changed.
   * @param listener listener for position change event, new position is passed to the listener
   * @return instance of Subscription, which allows to unsubscribe the listener
   * @see Subscription
   */
  Subscription addPositionListener(std::invocable<Position> auto &&listener) {
    return observableImpl.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
   * Get current position.
   * @return current position
   */
  [[nodiscard]] Position getPosition() const;
  /**
   * Set new position.
   * @param pos new position
   */
  virtual void setPosition(Position pos);

  virtual ~Positionable() = default;

 protected:
  /**
   * Checks if the new position is different from the current one. If it is, listeners are notified of this change.
   * @param pos new position
   */
  void updatePosition(Position pos);

 private:
  Position position;
  Observable_impl<Position> observableImpl;

  /**
   * Notify listeners of position change.
   * @param pos position to used as a parameter
   */
  void notifyPositionChanged(Position pos);
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_POSITIONABLE_H
