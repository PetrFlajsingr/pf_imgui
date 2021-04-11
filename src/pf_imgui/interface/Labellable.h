//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H

#include <pf_imgui/_export.h>
#include <string>
#include <fmt/format.h>

namespace pf::ui::ig {
class PF_IMGUI_EXPORT Labellable {
 public:
  explicit Labellable(std::string label);

  Labellable(Labellable &&other) noexcept;
  Labellable &operator=(Labellable &&other) noexcept;

  [[nodiscard]] const std::string &getLabel() const;
  void setLabel(const std::string &cap);
  void setLabel(const std::string &fmt, auto &&...args) requires (sizeof...(args) > 0) {
    setLabel(fmt::format(fmt, std::forward<decltype(args)>(args)...));
  }

  virtual ~Labellable() = default;

 private:
  std::string label_;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CAPTIONEDELEMENT_H
