/**
 * @file DockBuilder.h
 * @brief DockBuilder for DockSpace.
 * @author Petr Flajšingr
 * @date 12.4.22
 */

#ifndef PF_IMGUI_DOCKBUILDER_H
#define PF_IMGUI_DOCKBUILDER_H

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
class HorizontalSplitBuilder;
class VerticalSplitBuilder;
/**
 * @brief Builder for divided area.
 */
class SubDockBuilder {
  friend class DockBuilder;

 protected:
  explicit SubDockBuilder(Direction dir);

 public:
  SubDockBuilder(const SubDockBuilder &) = delete;
  SubDockBuilder &operator=(const SubDockBuilder &) = delete;
  SubDockBuilder(SubDockBuilder &&) = delete;
  SubDockBuilder &operator=(SubDockBuilder &&) = delete;

  /**
   * Split this area in two horizontally.
   * @param splitDirection direction of split
   * @return builder for the split off area
   */
  [[nodiscard]] HorizontalSplitBuilder &split(HorizontalDirection splitDirection);
  /**
   * Split this area in two vertically.
   * @param splitDirection direction of split
   * @return builder for the split off area
   */
  [[nodiscard]] VerticalSplitBuilder &split(VerticalDirection splitDirection);
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

 protected:
  void setSize(Size size);

 private:
  void run(ImGuiID &parentNodeId);

  Direction direction;
  float sizeRatio = 0.5f;
  std::vector<std::variant<details::DockWindowCmd, details::DockSizeCmd, std::unique_ptr<SubDockBuilder>>> subCommands;
};

class HorizontalSplitBuilder : public SubDockBuilder {
 protected:
  friend class DockBuilder;
  friend class SubDockBuilder;
  explicit HorizontalSplitBuilder(Direction dir) : SubDockBuilder(dir) {}

 public:
  /**
   * Set width of the divided off area.
   * @param width width
   */
  inline void setWidth(Width width) { SubDockBuilder::setSize(Size{width, 1}); }
};

class VerticalSplitBuilder : public SubDockBuilder {
 protected:
  friend class DockBuilder;
  friend class SubDockBuilder;
  explicit VerticalSplitBuilder(Direction dir) : SubDockBuilder(dir) {}

 public:
  /**
   * Set height of the divided off area.
   * @param height height
   */
  inline void setHeight(Height height) { SubDockBuilder::setSize(Size{1, height}); }
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
   * Split this area in two horizontally.
   * @param direction direction of split
   * @return builder for the split off area
   */
  [[nodiscard]] HorizontalSplitBuilder &split(HorizontalDirection direction);
  /**
   * Split this area in two vertically.
   * @param direction direction of split
   * @return builder for the split off area
   */
  [[nodiscard]] VerticalSplitBuilder &split(VerticalDirection direction);
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
#endif  //PF_IMGUI_DOCKBUILDER_H
