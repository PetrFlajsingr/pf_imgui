//
// Created by petr on 4/22/21.
//

#include "DragNDrop.h"
#include <pf_imgui/elements/Text.h>

namespace pf::ui::ig {

details::DragSourceBase::DragSourceBase(bool dragAllowed) : dragAllowed(dragAllowed) {}

bool details::DragSourceBase::isDragAllowed() const { return dragAllowed; }

void details::DragSourceBase::setDragAllowed(bool allowed) { dragAllowed = allowed; }

bool details::DragSourceBase::isDragged() const { return dragged; }

bool details::DragSourceBase::drag_impl(const std::string &typeName, const void *sourceData, std::size_t dataSize) {
  auto flags = ImGuiDragDropFlags_SourceAllowNullID;
  if (dragAllowed && ImGui::BeginDragDropSource(flags)) {
    dragged = true;
    ImGui::SetDragDropPayload(typeName.c_str(), sourceData, dataSize);
    if (tooltip != nullptr) { tooltip->render(); }
    ImGui::EndDragDropSource();
    auto payload = ImGui::GetDragDropPayload();
    return payload != nullptr && payload->IsDelivery();// check if this detection works
  } else {
    dragged = false;
  }
  return false;
}
bool details::DragSourceBase::hasDragTooltip() const { return tooltip != nullptr; }
Tooltip &details::DragSourceBase::getDragTooltip() {
  if (tooltip == nullptr) { throw StackTraceException("Drag tooltip doesn't exist."); }
  return *tooltip;
}
Tooltip &details::DragSourceBase::createDragTooltip() {
  tooltip = std::make_unique<Tooltip>(uniqueId());
  tooltipTextFmt = std::nullopt;
  return *tooltip;
}
void details::DragSourceBase::removeDragTooltip() {
  tooltip = nullptr;
  tooltipTextFmt = std::nullopt;
}
bool details::DragSourceBase::drag_impl_fmt(const std::string &typeName, const void *sourceData, std::size_t dataSize,
                                            std::string value) {
  if (!dragged && tooltipTextFmt.has_value()) {
    tooltipTextFmt->second->setText(fmt::format(tooltipTextFmt->first, value));
  }
  return drag_impl(typeName, sourceData, dataSize);
}
void details::DragSourceBase::createSimpleTooltip(std::string fmt, bool isValueFmt) {
  auto &tooltipText = createDragTooltip().createChild<Text>(uniqueId(), std::string(fmt));
  if (isValueFmt) {
    tooltipTextFmt = std::make_pair(fmt, &tooltipText);
  } else {
    tooltipTextFmt = std::nullopt;
  }
}

details::DropTargetBase::DropTargetBase(bool dropAllowed) : dropAllowed(dropAllowed) {}

bool details::DropTargetBase::isDropAllowed() const { return dropAllowed; }

void details::DropTargetBase::setDropAllowed(bool allowed) { dropAllowed = allowed; }

std::optional<void *> details::DropTargetBase::dropAccept_impl(const std::string &typeName) const {
  if (dropAllowed && ImGui::BeginDragDropTarget()) {
    auto payload = ImGui::AcceptDragDropPayload(typeName.c_str());// todo type conversions
    void *result = payload == nullptr ? nullptr : payload->Data;
    ImGui::EndDragDropTarget();
    if (result != nullptr) { return result; }
  }
  return std::nullopt;
}
}// namespace pf::ui::ig