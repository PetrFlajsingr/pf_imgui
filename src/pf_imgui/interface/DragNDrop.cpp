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
  if (!dragAllowed) { return false; }
  const auto startPayload = ImGui::GetDragDropPayload();
  auto didDrop = false;
  if (dragged && ownsPayload && startPayload == nullptr) {
    ownsPayload = false;
    dragged = false;
    didDrop = true;
  }
  auto flags = ImGuiDragDropFlags_SourceAllowNullID;
  if (ImGui::BeginDragDropSource(flags)) {
    dragged = true;
    ownsPayload = true;
    ImGui::SetDragDropPayload(typeName.c_str(), sourceData, dataSize, ImGuiCond_Once);
    if (tooltip != nullptr) { tooltip->render(); }
    ImGui::EndDragDropSource();
  } else {
    dragged = dragged && startPayload != nullptr && !startPayload->IsDelivery();
    ownsPayload = startPayload != nullptr;
  }
  return didDrop;
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
                                            const std::string &value) {
  if (!dragged && tooltipTextFmt.has_value()) {// TODO: check this condition
    tooltipTextFmt->second->setText(fmt::format(tooltipTextFmt->first, value));
  }
  return drag_impl(typeName, sourceData, dataSize);
}

void details::DragSourceBase::createSimpleTooltip(const std::string &fmt, bool isValueFmt) {
  auto &tooltipText = createDragTooltip().createChild<Text>(uniqueId(), std::string(fmt));
  if (isValueFmt) {
    tooltipTextFmt = std::make_pair(fmt, &tooltipText);
  } else {
    tooltipTextFmt = std::nullopt;
  }
}
bool details::DragSourceBase::hasFmtTooltip() const { return tooltipTextFmt.has_value(); }

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

void DragNDropGroup::frame() {
  if (std::ranges::any_of(sources, [](auto source) { return source->isDragged(); })) {
    if (!wasDraggedLastFrame) {
      std::ranges::for_each(targets, [](auto &t) {
        t.second = t.first->isDropAllowed();
        t.first->setDropAllowed(true);
      });
      wasDraggedLastFrame = true;
    }
  } else if (wasDraggedLastFrame) {
    std::ranges::for_each(targets, [](auto &t) { t.first->setDropAllowed(t.second); });
    wasDraggedLastFrame = false;
  }
}
}// namespace pf::ui::ig