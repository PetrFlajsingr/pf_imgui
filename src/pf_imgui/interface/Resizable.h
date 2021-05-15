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

template <typename T>
struct SizeDimension : public T {
  explicit SizeDimension(uint32_t value) : T(static_cast<float>(value)) {}
  static SizeDimension Fill(uint32_t margin) {
    return {-static_cast<float>(margin)};
  }
  static SizeDimension Auto() {
    return {0};
  }
};
namespace details {
  struct Width {
    float width;
  };
  struct Height {
    float height;
  };
}

using Width = SizeDimension<details::Width>;
using Height = SizeDimension<details::Height>;

/**
 * @brief Interface for resizable elements
 *
 * Provides a functionality to manage size for the subclass.
 */
class PF_IMGUI_EXPORT Resizable {
 public:
  /**
   * Construct Resizable with the given size
   * @param size size
   */
  explicit Resizable(const ImVec2 &size);

  Resizable(Resizable &&other) noexcept;
  Resizable &operator=(Resizable &&other) noexcept;

  /**
   * Get current size.
   * @return current size
   */
  [[nodiscard]] const ImVec2 &getSize() const;
  /**
   * Set new size.
   * @param s new size
   */
  virtual void setSize(const ImVec2 &s);

  /**
   * Add a listener for size changes.
   * @param listener listener called on size change
   * @return Subscription which allows of erasure of the listener @see Subscription
   */
  Subscription addSizeListener(std::invocable<ImVec2> auto listener) {
    return observableImpl.template addListener(listener);
  }

  virtual ~Resizable() = default;

 private:
  ImVec2 size;
  Observable_impl<ImVec2> observableImpl;

  void notifySizeChanged(ImVec2 newSize);
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
