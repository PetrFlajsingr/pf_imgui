/**
 * @file Resizable.h
 * @brief Interface for elements which can be resized.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
#define PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <string>

namespace pf::ui::ig {

namespace details {
template<typename T>
struct PF_IMGUI_EXPORT SizeDimension : public T {
  constexpr explicit(false) SizeDimension(std::same_as<uint32_t> auto value) : T(static_cast<float>(value)) {}
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
  constexpr static SizeDimension Fill(uint32_t margin = 1) { return {-static_cast<float>(margin)}; }
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
}// namespace details

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
   * @return
   */
  static Size Fill();
  /**
   * Fill on X, auto on Y
   * @return
   */
  static Size FillWidth();
  /**
   * Fill on Y, auto on X.
   * @return
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

/**
 * @brief Interface for resizable elements
 *
 * Provides a functionality to manage size for the subclass.
 */
class PF_IMGUI_EXPORT Resizable {
 public:
  /**
   * Construct Resizable with the given size
   * @param s size
   */
  explicit Resizable(const Size &s);

  Resizable(Resizable &&other) noexcept;
  Resizable &operator=(Resizable &&other) noexcept;

  /**
   * Get current size.
   * @return current size
   */
  [[nodiscard]] const Size &getSize() const;
  /**
   * Set new size.
   * @param s new size
   */
  virtual void setSize(const Size &s);
  /**
   * Add a listener for size changes.
   * @param listener listener called on size change
   * @return Subscription which allows of erasure of the listener @see Subscription
   */
  Subscription addSizeListener(std::invocable<Size> auto &&listener) {
    return observableImpl.addListener(std::forward<decltype(listener)>(listener));
  }

  virtual ~Resizable() = default;

 private:
  Size size;
  Observable_impl<Size> observableImpl;

  void notifySizeChanged(Size newSize);
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
