//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H

#include <fmt/format.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <string>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Labellable {
 public:
  explicit Labellable(std::string label);

  Labellable(Labellable &&other) noexcept;
  Labellable &operator=(Labellable &&other) noexcept;

  [[nodiscard]] const std::string &getLabel() const;
  void setLabel(const std::string &cap);
  void setLabel(const std::string &fmt, auto &&...args) requires(sizeof...(args) > 0) {
    setLabel(fmt::format(fmt, std::forward<decltype(args)>(args)...));
  }

  void setLabelVisible(Visibility newVisibility);
  void showLabel();
  void hideLabel();
  [[nodiscard]] bool isLabelVisible() const;

  virtual ~Labellable() = default;

 private:
  inline static std::string EMPTY_LABEL;
  std::string label_;
  Visibility visibility = Visibility::Visible;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
