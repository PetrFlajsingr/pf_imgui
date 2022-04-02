/**
* @file Size.h
* @brief Type representing size in pf_imgui.
* @author Petr Flajšingr
* @date 4.2.22
*/

#ifndef PF_IMGUI_SIZE_H
#define PF_IMGUI_SIZE_H

#include <concepts>
#include <cstdint>
#include <imgui.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

namespace details {
template<typename T>
struct PF_IMGUI_EXPORT SizeDimension : public T {
  constexpr explicit(false) SizeDimension(std::same_as<std::uint32_t> auto value) : T(static_cast<float>(value)) {}
  constexpr explicit(false) SizeDimension(std::same_as<int> auto value) : SizeDimension(static_cast<float>(value)) {}
  constexpr explicit(false) SizeDimension(std::same_as<float> auto value) : T(value) {}
  constexpr bool operator==(const SizeDimension &other) const { return T::value == other.value; }
  constexpr bool operator!=(const SizeDimension &other) const { return !(*this == other); }
  constexpr explicit(false) operator float() const { return T::value; }
  /**
   * Fill the dimension except for the margin.
   * @param margin
   * @return
   */
  constexpr static SizeDimension Fill(std::uint32_t margin = 1) { return {-static_cast<float>(margin)}; }
  /**
   * Automatic size detection.
   * @return
   */
  constexpr static SizeDimension Auto() { return {0}; }
};
struct PF_IMGUI_EXPORT Width {
  float value;
};
struct PF_IMGUI_EXPORT Height {
  float value;
};
}  // namespace details

using Width = details::SizeDimension<details::Width>;
using Height = details::SizeDimension<details::Height>;
/**
 * @brief Size to be used for element sizes.
 */
struct PF_IMGUI_EXPORT Size {
  Size(const Width &width, const Height &height);
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
  [[nodiscard]] ImVec2 asImVec() const;
  Width width;
  Height height;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_SIZE_H
