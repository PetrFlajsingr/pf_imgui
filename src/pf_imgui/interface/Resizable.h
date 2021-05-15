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

template<typename T>
struct SizeDimension : public T {
  SizeDimension(uint32_t value) : T(static_cast<float>(value)) {}
  bool operator==(const SizeDimension &other) const { return T::value == other.value; }
  bool operator!=(const SizeDimension &other) const { return !(*this == other); }
  static SizeDimension Fill(uint32_t margin = 1) { return {-margin}; }
  static SizeDimension Auto() { return {0}; }
};
namespace details {
struct Width {
  float value;
};
struct Height {
  float value;
};
}// namespace details

using Width = SizeDimension<details::Width>;
using Height = SizeDimension<details::Height>;
struct Size {
  Size(const Width &width, const Height &height);
  static Size Auto();
  static Size Fill();
  static Size FillWidth();
  bool operator==(const Size &rhs) const;
  bool operator!=(const Size &rhs) const;
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
  const Size &getSize() const;
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
  Subscription addSizeListener(std::invocable<Size> auto listener) {
    return observableImpl.template addListener(listener);
  }

  virtual ~Resizable() = default;

 private:
  Size size;
  Observable_impl<Size> observableImpl;

  void notifySizeChanged(Size newSize);
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
