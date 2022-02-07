/**
 * @file exceptions.h
 * @brief Exceptions for pf_imgui.
 * @author Petr Flajšingr
 * @date 13.4.21
 */

#ifndef PF_IMGUI_EXCEPTIONS_H
#define PF_IMGUI_EXCEPTIONS_H

#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

// @todo: get rid of the backward stuff/stacktrace
/**
 * @brief Exception thrown when a duplicate ID is added in the UI tree.
 */
class PF_IMGUI_EXPORT DuplicateIdException : public StackTraceException {
 public:
  explicit DuplicateIdException(const std::string_view &fmt, auto &&...args) : StackTraceException(fmt, args...) {}
};
/**
 * @brief Exception thrown when a an ID is not present.
 */
class PF_IMGUI_EXPORT IdNotFoundException : public StackTraceException {
 public:
  explicit IdNotFoundException(const std::string_view &fmt, auto &&...args) : StackTraceException(fmt, args...) {}
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_EXCEPTIONS_H
