/**
 * @file Label.h
 * @brief Class representing element's label.
 * @author Petr Flajšingr
 * @date 22.8.22
 */

#ifndef PF_IMGUI_LABEL_H
#define PF_IMGUI_LABEL_H

#include <pf_imgui/enums.h>
#include <pf_imgui/reactive/Observable_impl.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Label for UI elements.
 */
class Label {
 public:
  /**
   * Construct Label.
   * @param value initial value
   */
  explicit Label(std::string value = std::string{});

  Label &operator=(const Label &other);
  Label &operator=(const std::string &other);
  Label &operator=(std::string &&other);

  /**
   * Get current label value. Returns an empty string if visibility == Invisible.
   */
  [[nodiscard]] const std::string &get() const;

  void setVisibility(Visibility vis);
  void show();
  void hide();

  [[nodiscard]] Visibility getVisibility() const;

  /**
   * Add a listener to changes of label value
   * @param listener listener to register
   * @return Subscription allowing for listener cancellation
   */
  Subscription addListener(std::invocable<std::string_view> auto &&listener) {
    return observableImpl.addListener(std::forward<decltype(listener)>(listener));
  }

 private:
  inline static std::string EMPTY_LABEL;
  std::string val;
  Visibility visibility = Visibility::Visible;

  Observable_impl<std::string_view> observableImpl;
};

}  // namespace pf::ui::ig

#endif
