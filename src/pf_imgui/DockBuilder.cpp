//
// Created by xflajs00 on 12.04.2022.
//

#include "DockBuilder.h"

namespace pf::ui::ig {

SubDockBuilder::SubDockBuilder(Direction direction) : direction(direction) {}

SubDockBuilder &SubDockBuilder::split(Direction splitDirection) {
  auto subBuilder = std::make_unique<SubDockBuilder>(splitDirection);
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}

void SubDockBuilder::setSize(Size size) { subCommands.emplace_back(details::DockSizeCmd{size}); }

void SubDockBuilder::setSplitRatio(float ratio) { sizeRatio = ratio; }

void SubDockBuilder::setWindow(std::string name) { subCommands.emplace_back(details::DockWindowCmd{name}); }

void SubDockBuilder::run(ImGuiID &parentNodeId) {
  auto nodeId =
      ImGui::DockBuilderSplitNode(parentNodeId, static_cast<ImGuiDir>(direction), sizeRatio, nullptr, &parentNodeId);
  std::ranges::for_each(subCommands, [&](auto &val) {
    std::visit(Visitor{[&](const details::DockWindowCmd &cmd) {
                         ImGui::DockBuilderDockWindow(cmd.windowName.c_str(), nodeId);
                       },
                       [&](const std::unique_ptr<SubDockBuilder> &subDockBuilder) { subDockBuilder->run(nodeId); },
                       [&](const details::DockSizeCmd &sizeCmd) {
                         ImGui::DockBuilderSetNodeSize(nodeId, static_cast<ImVec2>(sizeCmd.size));
                       }},
               val);
  });
}

DockBuilder::DockBuilder(DockSpace &dockSpace) : dockSpaceRef(dockSpace) {}

SubDockBuilder &DockBuilder::split(Direction direction) {
  auto subBuilder = std::make_unique<SubDockBuilder>(direction);
  auto result = subBuilder.get();
  subCommands.emplace_back(std::move(subBuilder));
  return *result;
}

void DockBuilder::setSize(Size size) { subCommands.emplace_back(details::DockSizeCmd{size}); }

void DockBuilder::setWindow(std::string name) { subCommands.emplace_back(details::DockWindowCmd{name}); }

void DockBuilder::run() {
  auto dockspaceId = dockSpaceRef.getId();
  ImGui::DockBuilderRemoveNode(dockspaceId);
  ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);

  std::ranges::for_each(subCommands, [&](auto &val) {
    std::visit(
        Visitor{[&](const details::DockWindowCmd &cmd) { ImGui::DockBuilderDockWindow(cmd.windowName.c_str(), dockspaceId); },
                [&](const std::unique_ptr<SubDockBuilder> &subDockBuilder) { subDockBuilder->run(dockspaceId); },
                [&](const details::DockSizeCmd &sizeCmd) {
                  ImGui::DockBuilderSetNodeSize(dockspaceId, static_cast<ImVec2>(sizeCmd.size));
                }},
        val);
  });
}

}