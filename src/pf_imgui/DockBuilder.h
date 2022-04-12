/**
 * @file DockBuilder.h
 * @brief DockBuilder for DockSpace.
 * @author Petr Flajšingr
 * @date 12.4.22
 */

#ifndef IMGUI_EXPERIMENTS_DOCKBUILDER_H
#define IMGUI_EXPERIMENTS_DOCKBUILDER_H

#include <imgui_internal.h>
#include <memory>
#include <pf_common/Visitor.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/elements/DockSpace.h>
#include <pf_imgui/enums.h>
#include <string>
#include <variant>
#include <vector>

namespace pf::ui::ig {

namespace details {
struct DockWindowCmd {
  Window &window;
};

struct DockSizeCmd {
  Size size;
};
}  // namespace details

/**
 * @brief Builder for divided area.
 */
class SubDockBuilder {
  friend class DockBuilder;
  explicit SubDockBuilder(Direction direction);

 public:
  SubDockBuilder(const SubDockBuilder &) = delete;
  SubDockBuilder &operator=(const SubDockBuilder &) = delete;
  SubDockBuilder(SubDockBuilder &&) = delete;
  SubDockBuilder &operator=(SubDockBuilder &&) = delete;

  /**
   * Split this area in two.
   * @param direction direction of split
   * @return builder for the split off area
   */
  SubDockBuilder &split(Direction direction);
  /**
   * Set size of the new area.
   * @param size size of the new area
   */
  void setSize(Size size);
  /**
   * Set size of split off area as percentage. @warning DOES NOT WORK CURRENTLY
   * @param ratio split ratio
   */
  void setSplitRatio(float ratio);
  /**
   * Set Window to be placed within this area. If you insert multiple Windows, they'll be in docking tabs.
   * @param window Window to place
   */
  void setWindow(Window &window);

 private:
  void run(ImGuiID &parentNodeId);

  Direction direction;
  float sizeRatio = 0.5f;
  std::vector<std::variant<details::DockWindowCmd, details::DockSizeCmd, std::unique_ptr<SubDockBuilder>>> subCommands;
};

// TODO: set initial dockspace size properly so ratio functionality can be used
/**
 * @brief Builder for DockSpace.
 */
class DockBuilder {
  friend class ImGuiInterface;
  explicit DockBuilder(DockSpace &dockSpace);

 public:
  DockBuilder(const DockBuilder &) = delete;
  DockBuilder &operator=(const DockBuilder &) = delete;
  DockBuilder(DockBuilder &&) = delete;
  DockBuilder &operator=(DockBuilder &&) = delete;
  /**
   * Split this area in two.
   * @param direction direction of split
   * @return builder for the split off area
   */
  SubDockBuilder &split(Direction direction);
  /**
   * Set size of the new area.
   * @param size size of the new area
   */
  void setSize(Size size);
  /**
   * Set Window to be placed within this area. If you insert multiple Windows, they'll be in docking tabs.
   * @param window Window to place
   */
  void setWindow(Window &window);

 private:
  void run();

  DockSpace &dockSpaceRef;
  std::vector<std::variant<details::DockWindowCmd, details::DockSizeCmd, std::unique_ptr<SubDockBuilder>>> subCommands;
};

}  // namespace pf::ui::ig
#endif  //IMGUI_EXPERIMENTS_DOCKBUILDER_H
