//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H
#define PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Element {
 public:
  explicit Element(std::string elementName);
  virtual ~Element() = default;
  Element(Element &&other) noexcept;
  Element &operator=(Element &&other) noexcept;
  Element(const Element &) = delete;
  Element &operator=(const Element &) = delete;

  [[nodiscard]] const std::string &getName() const;

  [[nodiscard]] Visibility getVisibility() const;
  void setVisibility(Visibility visibility);

  [[nodiscard]] Enabled getEnabled() const;
  void setEnabled(Enabled eleState);

  void render();

 protected:
  virtual void renderImpl() = 0;
  Enabled enabled = Enabled::Yes;

 private:
  std::string name;
  Visibility visibility = Visibility::Visible;
  bool isMoved = false;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H