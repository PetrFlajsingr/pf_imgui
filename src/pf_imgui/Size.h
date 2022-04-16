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
#include <cstdint>
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
  Size(Width width, Height height);
  /**
   * Conversion constructor.
   * @param vec size as ImVec2
   */
  explicit(false) Size(ImVec2 vec);
  /**
   * Automatic size detection.
   * @return
   */
  static Size Auto();
  /**
   * Fill.
   */
  static Size Fill();
  /**
   * Fill on X, auto on Y.
   */
  static Size FillWidth();
  /**
   * Fill on Y, auto on X.
   */
  static Size FillHeight();
  bool operator==(const Size &rhs) const;
  bool operator!=(const Size &rhs) const;
  /**
   * Convert to ImVec, mostly for internal use.
   * @return size as ImVec
   */
  explicit operator ImVec2() const;
  Width width;
  Height height;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_SIZE_H
