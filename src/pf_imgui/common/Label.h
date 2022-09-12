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
  explicit(false) Label(std::string value = std::string{});
  Label(const Label &other) = default;

  Label &operator=(const Label &other);
  Label &operator=(const std::string &other);
  Label &operator=(std::string &&other);

  [[nodiscard]] bool operator==(const Label &other) const;

  /**
   * Get current label value. Returns an empty string if visibility == Invisible.
   */
  [[nodiscard]] const std::string &get() const;

  void setVisibility(Visibility vis);
  void show();
  void hide();

  [[nodiscard]] Visibility getVisibility() const;

 private:
  inline static std::string EMPTY_LABEL;
  std::string val;
  Visibility visibility = Visibility::Visible;
};

}  // namespace pf::ui::ig

#endif
