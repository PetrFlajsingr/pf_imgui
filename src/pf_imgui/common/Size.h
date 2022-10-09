/**
 * @file Size.h
 * @brief Type representing size in pf_imgui.
 * @author Petr Flajšingr
 * @date 4.2.22
 */

#ifndef PF_IMGUI_SIZE_H
#define PF_IMGUI_SIZE_H

#include <compare>
#include <concepts>
#include <imgui.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Width {
 public:
  constexpr explicit(false) Width(float width) : value(width) {}

  constexpr auto operator<=>(const Width &) const = default;
  [[nodiscard]] constexpr Width operator+(Width rhs) const { return {value + rhs.value}; }
  [[nodiscard]] constexpr Width operator-(Width rhs) const { return {value - rhs.value}; }
  [[nodiscard]] constexpr Width &operator+=(Width rhs) {
    value += rhs.value;
    return *this;
  }
  [[nodiscard]] constexpr Width &operator-=(Width rhs) {
    value -= rhs.value;
    return *this;
  }

  constexpr explicit operator float() const { return value; }
  /**
   * Fill the except for the margin.
   * @param margin
   * @return
   */
  [[nodiscard]] constexpr static Width Fill(std::uint32_t margin = 1) { return {-static_cast<float>(margin)}; }
  /**
   * Automatic size detection.
   * @return
   */
  [[nodiscard]] constexpr static Width Auto() { return {0}; }

 private:
  float value;
};

class PF_IMGUI_EXPORT Height {
 public:
  constexpr explicit(false) Height(float height) : value(height) {}

  constexpr auto operator<=>(const Height &) const = default;
  [[nodiscard]] constexpr Height operator+(Height rhs) const { return {value + rhs.value}; }
  [[nodiscard]] constexpr Height operator-(Height rhs) const { return {value - rhs.value}; }
  [[nodiscard]] constexpr Height &operator+=(Height rhs) {
    value += rhs.value;
    return *this;
  }
  [[nodiscard]] constexpr Height &operator-=(Height rhs) {
    value -= rhs.value;
    return *this;
  }

  constexpr explicit operator float() const { return value; }
  /**
   * Fill the except for the margin.
   * @param margin
   * @return
   */
  [[nodiscard]] constexpr static Height Fill(std::uint32_t margin = 1) { return {-static_cast<float>(margin)}; }
  /**
   * Automatic size detection.
   * @return
   */
  [[nodiscard]] constexpr static Height Auto() { return {0}; }

 private:
  float value;
};

/**
 * @brief Size to be used for element sizes.
 */
struct PF_IMGUI_EXPORT Size {
  constexpr Size(Width initialWidth, Height initialHeight) : width(initialWidth), height(initialHeight) {}
  /**
   * Conversion constructor.
   * @param vec size as ImVec2
   */
  constexpr explicit(false) Size(ImVec2 vec) : width(vec.x), height(vec.y) {}
  /**
   * Automatic size detection.
   * @return
   */
  constexpr static Size Auto() { return {Width::Auto(), Height::Auto()}; }
  /**
   * Fill.
   */
  constexpr static Size Fill() { return {Width::Fill(), Height::Fill()}; }
  /**
   * Fill on X, auto on Y.
   */
  constexpr static Size FillWidth() { return {Width::Fill(), Height::Auto()}; }
  /**
   * Fill on Y, auto on X.
   */
  constexpr static Size FillHeight() { return {Width::Auto(), Height::Fill()}; }
  constexpr bool operator==(const Size &rhs) const { return width == rhs.width && height == rhs.height; }
  constexpr bool operator!=(const Size &rhs) const { return !(rhs == *this); }
  /**
   * Convert to ImVec, mostly for internal use.
   * @return size as ImVec
   */
  constexpr explicit operator ImVec2() const { return ImVec2{static_cast<float>(width), static_cast<float>(height)}; }
  Width width;
  Height height;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_SIZE_H
