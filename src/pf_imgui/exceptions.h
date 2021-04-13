//
// Created by petr on 4/13/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_EXCEPTIONS_H
#define PF_IMGUI_SRC_PF_IMGUI_EXCEPTIONS_H

#include <pf_common/exceptions/StackTraceException.h>

namespace pf::ui::ig {

/**
 * Exception thrown when a duplicate ID is added in the UI tree.
 */
class DuplicateIdException : public StackTraceException {
 public:
  explicit DuplicateIdException(const std::string_view &fmt, auto &&...args)
      : StackTraceException(StackTraceException::fmt(fmt, args...)) {}
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_EXCEPTIONS_H
