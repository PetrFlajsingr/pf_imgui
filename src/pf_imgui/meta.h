//
// Created by petr on 4/12/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_META_H
#define PF_IMGUI_SRC_PF_IMGUI_META_H

#include <pf_common/specializations.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

template <typename T>
constexpr auto IsValueObservable = derived_specialization_of<T, ValueObservable>;
}

#endif//PF_IMGUI_SRC_PF_IMGUI_META_H
