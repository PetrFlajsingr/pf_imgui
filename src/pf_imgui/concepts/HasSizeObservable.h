/**
 * @file HasSizeObservable.h
 * @brief Concept checking for member `Observable<Size> size`.
 * @author Petr Flajšingr
 * @date 4.9.22
 */

#ifndef PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H
#define PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H

#include <concepts>
#include <pf_imgui/Size.h>
#include <pf_imgui/reactive/Observable.h>

namespace pf::ui::ig {

template<typename T>
concept HasSizeObservable = requires(T t) {
                              { &t.size } -> std::same_as<Observable<Size>*>;
                            };

}

#endif  //PF_IMGUI_CONCEPTS_HASSIZEOBSERVABLE_H
