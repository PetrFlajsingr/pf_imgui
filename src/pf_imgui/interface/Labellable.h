/**
 * @file Labellable.h
 * @brief Class providing label support.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_CAPTIONEDELEMENT_H
#define PF_IMGUI_INTERFACE_CAPTIONEDELEMENT_H

#include <fmt/format.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Interface for labellable elements
 *
 * Provides a functionality to manage label for the subclass.
 */
class PF_IMGUI_EXPORT Labellable {
 public:
  /**
   * Construct Labellable with given label
   * @param label label to be displayed
   */
  explicit Labellable(std::string label);

  Labellable(Labellable &&other) noexcept;
  Labellable &operator=(Labellable &&other) noexcept;

  /**
   * Get current label
   * @return label
   */
  [[nodiscard]] const std::string &getLabel() const;
  /**
   * Set new label with formatting bz fmt::format
   * @param fmt format string
   * @param args arguments for format string
   */
  template<typename... Args>
  void setLabel(fmt::format_string<Args...> format, Args &&...args) {
    setLabel(fmt::format(format, std::forward<Args>(args)...));
  }
  inline void setLabel(const std::string &newLabel) {
    label_ = newLabel;
    observableImpl.notify(label_);
  }

  /**
   * Add a listener to changes of label value
   * @param listener listener to register
   * @return Subscription allowing for listener cancellation
   */
  Subscription addLabelListener(std::invocable<std::string_view> auto &&listener) {
    return observableImpl.addListener(std::forward<decltype(listener)>(listener));
  }

  /**
   * Set visibility of the label
   * @param newVisibility
   */
  void setLabelVisible(Visibility newVisibility);
  /**
   * Set label visibility to Invisible.
   */
  void showLabel();
  /**
   * Set label visibility to Visible.
   */
  void hideLabel();
  /**
   * Check label visibility.
   * @return true if the label is visible, false otherwise
   */
  [[nodiscard]] bool isLabelVisible() const;

  virtual ~Labellable() = default;

 private:
  inline static std::string EMPTY_LABEL;
  std::string label_;
  Visibility visibility = Visibility::Visible;
  Observable_impl<std::string_view> observableImpl;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_CAPTIONEDELEMENT_H
