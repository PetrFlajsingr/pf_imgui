//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_ELEMENTS_BUTTON_H
#define PF_IMGUI_ELEMENTS_BUTTON_H

#include "interface/Clickable.h"
#include "interface/ItemElement.h"
#include "interface/Labellable.h"
#include "interface/Resizable.h"
#include <functional>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Button : public ItemElement, public Labellable, public Resizable, public Clickable {
 public:
  Button(const std::string &name, std::string label, ButtonType buttonType = ButtonType::Normal,
         const ImVec2 &size = {0, 0});

  [[nodiscard]] ButtonType getType() const;

  void setType(ButtonType type);

 protected:
  void renderImpl() override;

 private:
  ButtonType type;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_ELEMENTS_BUTTON_H
