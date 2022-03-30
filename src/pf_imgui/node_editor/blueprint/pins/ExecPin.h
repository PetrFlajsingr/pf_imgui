//
// Created by xflajs00 on 29.03.2022.
//

#ifndef IMGUI_EXPERIMENTS_EXECPIN_H
#define IMGUI_EXPERIMENTS_EXECPIN_H

#include "Pin.h"
#include <imgui_internal.h>

namespace pf::ui::ig::bp {

class ExecPin : public Pin {
 public:
  ExecPin(const std::string &name, const std::string &label, const ImColor &pinColor);

  [[nodiscard]] bool acceptsLinkWith(pf::ui::ig::Pin &other) const override;

 protected:
  void renderIcon() override;

  void addLink(std::shared_ptr<Link> link) override;
};

}  // namespace pf::ui::ig::bp

#endif  //IMGUI_EXPERIMENTS_EXECPIN_H
