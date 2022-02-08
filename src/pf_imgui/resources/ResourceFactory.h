//
// Created by petr.flajsingr on 2/8/2022.
//

#ifndef PF_IMGUI_RESOURCEFACTORY_H
#define PF_IMGUI_RESOURCEFACTORY_H

#include "Resource.h"
#include <utility>
#include <memory>

namespace pf::ui::ig {

template<typename T>
inline std::unique_ptr<ConstResource<T>> makeConstResource(T &&value) {
  return std::make_unique<ConstResource<T>>(std::forward<T>(value));
}

}

#endif  //PF_IMGUI_RESOURCEFACTORY_H
