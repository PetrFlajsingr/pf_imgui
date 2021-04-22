//
// Created by petr on 4/20/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H

#include <algorithm>
#include <concepts>
#include <fmt/ostream.h>
#include <imgui.h>
#include <optional>
#include <pf_common/concepts/StringConvertible.h>
#include <pf_imgui/elements/Tooltip.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/unique_id.h>
#include <ranges>
#include <static_type_info.h>
#include <vector>

namespace pf::ui::ig {

// TODO: type conversions, user customisation
// TODO: type IDs

namespace details {
/**
 * @brief Base class for templated DragSource<T>
 *
 * Enables content dragging from this element. Provides an ability to add tooltip to dragged data.
 */
class DragSourceBase {
 public:
  /**
   * Construct DragSourceBase.
   * @param dragAllowed enable/disable drag
   */
  explicit DragSourceBase(bool dragAllowed);
  /**
   * Check if dragging is allowed.
   * @return true if the drag is allowed
   */
  [[nodiscard]] bool isDragAllowed() const;
  /**
   * Enable/disable dragging.
   * @param allowed
   */
  void setDragAllowed(bool allowed);
  /**
   * Check if content is currently dragged.
   * @return true if content is dragged
   */
  [[nodiscard]] bool isDragged() const;

  /**
   * Check if dragging has an active tooltip.
   * @return true if the tooltip is active, false otherwise
   */
  [[nodiscard]] bool hasDragTooltip() const;
  /**
   * Get the drag tooltip if it exists.
   * @return tooltip of drag
   *
   * @throws StacktraceException if the tooltip doesn't exist
   */
  [[nodiscard]] Tooltip &getDragTooltip();
  /**
   * Create an instance of tooltip for drag to be filled with elements by the user.
   * @return reference to the newly created tooltip
   */
  [[nodiscard]] Tooltip &createDragTooltip();
  /**
   * Removes drag's tooltip if it is assigned. If the tooltip doesn't exist, nothing happens.
   */
  void removeDragTooltip();

  virtual ~DragSourceBase() = default;

 protected:
  /**
   * Run drag source code if allowed. Renders the tooltip as well.
   * @param typeName type of transferred data
   * @param sourceData transferred data
   * @param dataSize size of transferred data
   * @return true if content is being dragged, false otherwise
   */
  bool drag_impl(const std::string &typeName, const void *sourceData, std::size_t dataSize);
  /**
   * Run drag source code if allowed. Renders the tooltip as well.
   * @param typeName type of transferred data
   * @param sourceData transferred data
   * @param dataSize size of transferred data
   * @param value string to be inserted into simple tooltip
   * @return true if content is being dragged, false otherwise
   */
  bool drag_impl_fmt(const std::string &typeName, const void *sourceData, std::size_t dataSize, std::string value);
  bool dragAllowed;

  /**
   * Create a tooltip possibly with format capability.
   * @param fmt text to show/format string in case of isValueFmt
   * @param isValueFmt true if fmt contains {} for formatting
   */
  void createSimpleTooltip(std::string fmt, bool isValueFmt);

 private:
  bool dragged = false;
  std::unique_ptr<Tooltip> tooltip = nullptr;
  std::optional<std::pair<std::string, Text *>> tooltipTextFmt;
};

/**
 * @brief Base class for templated DropTarget<T>
 *
 * Enables content drop from DragSource.
 */
class DropTargetBase {
 public:
  /**
   * Construct DropTargetBase.
   * @param dropAllowed enable/disable as drop target
   */
  explicit DropTargetBase(bool dropAllowed);
  /**
   * Check whether this object functions as a drop target.
   * @return true if this object functions as a drop target
   */
  [[nodiscard]] bool isDropAllowed() const;
  /**
   * Enable/disable this object as drop target.
   * @param allowed
   */
  void setDropAllowed(bool allowed);

  virtual ~DropTargetBase() = default;

 protected:
  /**
   * Implementation of drop acceptance. Controlled by dropAllowed.
   * @param typeName
   * @return
   */
  [[nodiscard]] std::optional<void *> dropAccept_impl(const std::string &typeName) const;

  bool dropAllowed;
};
}// namespace details
class DragNDropGroup;

/**
 * @brief Class providing the ability for derived to be a source of data for drag and drop operation.
 * @tparam T type of transferred data
 *
 * You have to use drag(...) function in order to activate drag source.
 */
template<typename T>
class DragSource : public details::DragSourceBase {
 public:
  /**
   * Construct DragSource.
   * @param dragAllowed enable/disable drag
   */
  explicit DragSource(bool dragAllowed) : DragSourceBase(dragAllowed) {}

  /**
   * Set simple tooltip for dragging. If the text contains '{}' and T is ToStringConvertible than the value transferred is formatted to the string.
   * @param text text on tooltip
   */
  void setDragTooltip(std::string_view text) {
    createSimpleTooltip(std::string(text), text.find("{}") != std::string_view::npos);
  }

 protected:
  /**
   * Call this method to start potential dragging.
   * @param sourceData data to be transferred
   * @return true if the value has been transferred
   */
  bool drag(const T &sourceData) {
    if constexpr (ToStringConvertible<T>) {
      return drag_impl_fmt(std::string(static_type_info::getTypeName<T>().substr(0, 32)),
                           reinterpret_cast<const void *>(&sourceData), sizeof(const T), toString(sourceData));
    } else {
      return drag_impl(std::string(static_type_info::getTypeName<T>().substr(0, 32)),
                       reinterpret_cast<const void *>(&sourceData), sizeof(const T));
    }
  }
};

/**
 * @brief Target for drag and drop operation.
 * @tparam T type of data which this object accepts
 */
template<typename T>
class DropTarget : public details::DropTargetBase {
 public:
  /**
   * Construct DropTarget.
   * @param dropAllowed enable/disable drop target
   */
  explicit DropTarget(bool dropAllowed) : DropTargetBase(dropAllowed) {}

 protected:
  /**
   * Call this method in order to accept potential transferred data.
   * @return std::nullopt if no value was accepted, the value otherwise
   */
  std::optional<T> dropAccept() {
    const auto dropResult = dropAccept_impl(std::string(static_type_info::getTypeName<T>().substr(0, 32)));
    if (dropResult.has_value()) { return *reinterpret_cast<const T *>(*dropResult); }
    return std::nullopt;
  }
};

/**
 * @brief Group of components that activate drop accept when drag is started.
 */
class DragNDropGroup {
 public:
  DragNDropGroup() = default;
  void frame();

  /**
   * Add elements to the group.
   * @tparam T type of the first element
   * @tparam Args other elements
   * @param arg1
   * @param args
   */
  template<typename T, typename... Args>
  void add(T &arg1, Args &...args) requires(
      std::derived_from<T, details::DropTargetBase> || std::derived_from<T, details::DragSourceBase>) {
    if constexpr (std::derived_from<T, details::DropTargetBase>) { targets.emplace_back(&arg1, false); }
    if constexpr (std::derived_from<T, details::DragSourceBase>) { sources.emplace_back(&arg1); }
    if constexpr (sizeof...(Args) > 0) { add(std::forward<Args &>(args)...); }
  }

  /**
   * Remove elements from the group.
   * @tparam T
   * @tparam Args
   * @param arg1
   * @param args
   */
  template<typename T, typename... Args>
  void remove(T &arg1, Args &...args) requires(
      std::derived_from<T, details::DropTargetBase> || std::derived_from<T, details::DragSourceBase>) {
    if constexpr (std::derived_from<T, details::DropTargetBase>) {
      std::ranges::remove_if(targets, [&arg1](const auto &t) { return t.first == &arg1; });
    }
    if constexpr (std::derived_from<T, details::DragSourceBase>) {
      std::ranges::remove_if(sources, [&arg1](const auto &s) { return s == &arg1; });
    }
    if constexpr (sizeof...(Args) > 0) { remove(std::forward<Args &>(args)...); }
  }

 private : std::vector<details::DragSourceBase *> sources;
  std::vector<std::pair<details::DropTargetBase *, bool>> targets;
  bool wasDraggedLastFrame = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H
