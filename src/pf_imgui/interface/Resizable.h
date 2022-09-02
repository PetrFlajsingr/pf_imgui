/**
 * @file Resizable.h
 * @brief Interface for elements which can be resized.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_RESIZABLE_H
#define PF_IMGUI_INTERFACE_RESIZABLE_H

#include <imgui.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/reactive/Observable_impl.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

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
  virtual ~Resizable() = default;

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

 protected:
  /**
   * Set size without notifying observers.
   */
  void setSizeInner(Size newSize);

 private:
  Size size;
  Observable_impl<Size> observableImpl;

  void notifySizeChanged(Size newSize);
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_RESIZABLE_H
