//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H
#define PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H

#include "Renderable.h"
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Element : public Renderable {
 public:
  explicit Element(std::string elementName);
  Element(Element &&other) noexcept;
  Element &operator=(Element &&other) noexcept;
  Element(const Element &) = delete;
  Element &operator=(const Element &) = delete;

  [[nodiscard]] const std::string &getName() const;

  void render() override;

 private:
  std::string name;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_INTERFACE_ELEMENT_H