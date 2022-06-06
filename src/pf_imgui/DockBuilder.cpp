//
// Created by xflajs00 on 12.04.2022.
//

#include "DockBuilder.h"
#include <pf_imgui/dialogs/Window.h>

namespace pf::ui::ig {

SubDockBuilder::SubDockBuilder(Direction direction) : direction(direction) {}

HorizontalSplitBuilder &SubDockBuilder::split(HorizontalDirection splitDirection) {
  auto subBuilder = std::unique_ptr<HorizontalSplitBuilder>{new HorizontalSplitBuilder{static_cast<Direction>(splitDirection)}}; //-V824
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}

VerticalSplitBuilder &SubDockBuilder::split(VerticalDirection splitDirection) {
  auto subBuilder = std::unique_ptr<VerticalSplitBuilder>{new VerticalSplitBuilder{static_cast<Direction>(splitDirection)}}; //-V824
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}


void SubDockBuilder::setSize(Size size) { subCommands.emplace_back(details::DockSizeCmd{size}); }

void SubDockBuilder::setSplitRatio(float ratio) { sizeRatio = ratio; }

void SubDockBuilder::setWindow(Window &window) { subCommands.emplace_back(details::DockWindowCmd{window}); }

void SubDockBuilder::run(ImGuiID &parentNodeId) {
  auto nodeId =
      ImGui::DockBuilderSplitNode(parentNodeId, static_cast<ImGuiDir>(direction), sizeRatio, nullptr, &parentNodeId);
  std::ranges::for_each(subCommands, [&](auto &val) {
    std::visit(Visitor{[&](const details::DockWindowCmd &cmd) {
                         ImGui::DockBuilderDockWindow(cmd.window.getImGuiName().c_str(), nodeId);
                       },
                       [&](const std::unique_ptr<SubDockBuilder> &subDockBuilder) { subDockBuilder->run(nodeId); },
                       [&](const details::DockSizeCmd &sizeCmd) {
                         ImGui::DockBuilderSetNodeSize(nodeId, static_cast<ImVec2>(sizeCmd.size));
                       }},
               val);
  });
}

DockBuilder::DockBuilder(DockSpace &dockSpace) : dockSpaceRef(dockSpace) {}

HorizontalSplitBuilder &DockBuilder::split(HorizontalDirection direction) {
  auto subBuilder = std::unique_ptr<HorizontalSplitBuilder>{new HorizontalSplitBuilder{static_cast<Direction>(direction)}}; //-V824
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}

VerticalSplitBuilder &DockBuilder::split(VerticalDirection direction) {
  auto subBuilder = std::unique_ptr<VerticalSplitBuilder>{new VerticalSplitBuilder{static_cast<Direction>(direction)}}; //-V824
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}

void DockBuilder::setSize(Size size) { subCommands.emplace_back(details::DockSizeCmd{size}); }

void DockBuilder::setWindow(Window &window) { subCommands.emplace_back(details::DockWindowCmd{window}); }

void DockBuilder::run() {
  auto dockspaceId = dockSpaceRef.getId();
  ImGui::DockBuilderRemoveNode(dockspaceId);
  ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

  std::ranges::for_each(subCommands, [&](auto &val) {
    std::visit(Visitor{[&](const details::DockWindowCmd &cmd) {
                         ImGui::DockBuilderDockWindow(cmd.window.getImGuiName().c_str(), dockspaceId);
                       },
                       [&](const std::unique_ptr<SubDockBuilder> &subDockBuilder) { subDockBuilder->run(dockspaceId); },
                       [&](const details::DockSizeCmd &sizeCmd) {
                         ImGui::DockBuilderSetNodeSize(dockspaceId, static_cast<ImVec2>(sizeCmd.size));
                       }},
               val);
  });
}

}  // namespace pf::ui::ig