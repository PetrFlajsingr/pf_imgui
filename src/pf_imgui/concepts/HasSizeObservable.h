/**
 * @file HasSizeObservable.h
 * @brief Concept checking for member `Observable<Size> size`.
 * @author Petr Flajšingr
 * @date 4.9.22
 */

#ifndef PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H
#define PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H

#include <concepts>
#include <pf_imgui/common/Size.h>
#include <pf_common/specializations.h>
#include <pf_imgui/reactive/Observable.h>

namespace pf::ui::ig {

template<typename T>
concept HasSizeObservable = requires(T t) {
                              { t.size };
                              direct_specialization_of<std::remove_cvref<decltype(t.size)>, ObservableProperty>;
                              std::same_as<typename decltype(t.size)::value_type, Size>;
                            };

}

#endif  //PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H
