//
// Created by Petr on 6/17/2022.
//

#include "ElementBase.h"
#include <imgui_internal.h>
#include <pf_common/RAII.h>

namespace pf::ui::ig {

ElementBase::ElementBase(const std::string &name) : Renderable(name) {}

ElementBase::~ElementBase() { observableDestroy.notify(); }

ElementBase::ElementBase(ElementBase &&other) noexcept  //-V730
    : Renderable(std::move(other)), observableDestroy(std::move(other.observableDestroy)) {}

ElementBase &ElementBase::operator=(ElementBase &&other) noexcept {
  observableDestroy = std::move(other.observableDestroy);
  Renderable::operator=(std::move(other));
  return *this;
}


}  // namespace pf::ui::ig