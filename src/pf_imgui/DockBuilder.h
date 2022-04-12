//
// Created by xflajs00 on 12.04.2022.
//

#ifndef IMGUI_EXPERIMENTS_DOCKBUILDER_H
#define IMGUI_EXPERIMENTS_DOCKBUILDER_H

#include <pf_imgui/elements/DockSpace.h>
#include <imgui_internal.h>
#include <memory>
#include <pf_common/Visitor.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/enums.h>
#include <string>
#include <variant>
#include <vector>

namespace pf::ui::ig {

namespace details {
struct DockWindowCmd {
  std::string windowName;
};

struct DockSizeCmd {
  Size size;
};
}  // namespace details

class SubDockBuilder {
 public:
  explicit SubDockBuilder(Direction direction);

  SubDockBuilder &split(Direction direction);

  void setSize(Size size);
  void setSplitRatio(float ratio);

  void setWindow(std::string name);

  void run(ImGuiID &parentNodeId);

 private:
  Direction direction;
  float sizeRatio = 0.5f;
  std::vector<std::variant<details::DockWindowCmd, details::DockSizeCmd, std::unique_ptr<SubDockBuilder>>> subCommands;
};


// TODO: set initial dockspace size properly so ratio functionality can be used
class DockBuilder {
  friend class ImGuiInterface;

 public:
  explicit DockBuilder(DockSpace &dockSpace);

  SubDockBuilder &split(Direction direction);

  void setSize(Size size);

  void setWindow(std::string name);

  void run();

 private:
  DockSpace &dockSpaceRef;
  std::vector<std::variant<details::DockWindowCmd, details::DockSizeCmd, std::unique_ptr<SubDockBuilder>>> subCommands;
};




}  // namespace pf::ui::ig
#endif  //IMGUI_EXPERIMENTS_DOCKBUILDER_H
