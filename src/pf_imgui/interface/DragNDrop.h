//
// Created by petr on 4/20/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H
#define PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H

#include <algorithm>
#include <concepts>
#include <imgui.h>
#include <optional>
#include <pf_imgui/elements/Tooltip.h>
#include <ranges>
#include <static_type_info.h>
#include <vector>

namespace pf::ui::ig {

// TODO: type conversions, user customisation
// TODO: type IDs
// TODO: some tooltip formatting for transported value

namespace details {
class DragSourceBase {
 public:
  inline explicit DragSourceBase(bool dragAllowed) : dragAllowed(dragAllowed) {}
  [[nodiscard]] inline bool isDragAllowed() const { return dragAllowed; }
  inline void setDragAllowed(bool allowed) { dragAllowed = allowed; }
  [[nodiscard]] inline bool isDragged() const { return dragged; }

  virtual ~DragSourceBase() = default;
  // TODO: tooltip

 protected:
  inline bool drag_impl(const std::string &typeName, void *sourceData, std::size_t dataSize) {
    auto flags = ImGuiDragDropFlags_SourceAllowNullID;
    if (dragAllowed && ImGui::BeginDragDropSource(flags)) {
      dragged = true;
      ImGui::SetDragDropPayload(typeName.c_str(), sourceData, dataSize);
      if (tooltip.has_value()) { tooltip->render(); }
      ImGui::EndDragDropSource();
      auto payload = ImGui::GetDragDropPayload();
      return payload != nullptr && payload->IsDelivery();// check if this detection works
    } else {
      dragged = false;
    }
    return false;
  }
  bool dragged = false;
  bool dragAllowed;
  std::optional<Tooltip> tooltip;
};

class DropTargetBase {
 public:
  inline explicit DropTargetBase(bool dropAllowed) : dropAllowed(dropAllowed) {}
  [[nodiscard]] inline bool isDropAllowed() const { return dropAllowed; }
  inline void setDropAllowed(bool allowed) { dropAllowed = allowed; }

  virtual ~DropTargetBase() = default;

 protected:
  [[nodiscard]] inline std::optional<void *> dropAccept_impl(const std::string &typeName) const {
    if (dropAllowed && ImGui::BeginDragDropTarget()) {
      auto payload = ImGui::AcceptDragDropPayload(typeName.c_str());// todo type conversions
      void *result = payload == nullptr ? nullptr : payload->Data;
      ImGui::EndDragDropTarget();
      if (result != nullptr) { return result; }
    }
    return std::nullopt;
  }

  bool dropAllowed;
};
}// namespace details
class DragNDropGroup;

template<typename T>
class DragSource : public details::DragSourceBase {
 public:
  explicit DragSource(bool dragAllowed) : DragSourceBase(dragAllowed) {}

 protected:
  bool drag(const T *sourceData) {
    const auto ptrPtr = &sourceData;
    return drag_impl(std::string(static_type_info::getTypeName<T>().substr(0, 32)),
                     reinterpret_cast<void *>(const_cast<T **>(ptrPtr)), sizeof(const T *));
  }
};

template<typename T>
class DropTarget : public details::DropTargetBase {
 public:
  explicit DropTarget(bool dropAllowed) : DropTargetBase(dropAllowed) {}

 protected:
  std::optional<T> dropAccept() {
    const auto dropResult = dropAccept_impl(std::string(static_type_info::getTypeName<T>().substr(0, 32)));
    if (dropResult.has_value()) { return **reinterpret_cast<const T **>(*dropResult); }
    return std::nullopt;
  }
};

class DragNDropGroup {
 public:
  explicit DragNDropGroup(details::DragSourceBase &src) : source(src) {}
  void frame() {
    if (source.isDragAllowed() && source.isDragged()) {
      if (!wasDraggedLastFrame) {
        std::ranges::for_each(targets, [](auto &t) { t.second = t.first->isDropAllowed(); });
      }
    } else if (wasDraggedLastFrame) {
      std::ranges::for_each(targets, [](auto &t) { t.first->setDropAllowed(t.second); });
      wasDraggedLastFrame = false;
    }
  }

  void addTarget(std::derived_from<details::DropTargetBase> auto &...target) {
    (targets.emplace_back(&target, false), ...);
  }
  void addTargets(std::ranges::range auto &&target) requires(
      std::derived_from<std::ranges::range_value_t<decltype(target)>, details::DropTargetBase>) {
    std::ranges::copy(target, std::back_inserter(targets));
  }
  void removeTarget(details::DropTargetBase &target) {
    std::ranges::remove_if(targets, [&target](const auto &t) { return t.first == &target; });
  }

 private:
  details::DragSourceBase &source;
  std::vector<std::pair<details::DropTargetBase *, bool>> targets;
  bool wasDraggedLastFrame = false;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_INTERFACE_DRAGNDROP_H
