/**
 * @file StackedLayout.h
 * @brief Layout providing switchable stacks.
 * @author Petr Flajšingr
 * @date 24.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_STACKEDLAYOUT_H
#define PF_IMGUI_LAYOUTS_STACKEDLAYOUT_H

#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Layout.h>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A layout that uses stacks to show different 'pages', of which only one can be shown.
 *
 * This layout serves as a multi-page layout.
 */
class PF_IMGUI_EXPORT StackedLayout : public Layout {
 public:
  /**
   * @brief Struct for construction of StackedLayout.
   */
  struct Config {
    using Parent = StackedLayout;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<Size> size;             /*!< Size of the element */
    bool showBorder = false;         /*!< Render border around layout's area */
  };
  /**
   * Construct StackedLayout
   * @param config construction args @see StackedLayout::Config
   */
  explicit StackedLayout(Config &&config);
  /**
   * @brief A container for data within the layout.
   */
  class Stack : public ElementContainer {
   public:
    explicit Stack(StackedLayout &owner);
    void setActive();

   private:
    StackedLayout &parent;
  };
  /**
   * Construct StackedLayout.
   * @param elementName ID of the layout
   * @param initialSize size of the layout
   * @param showBorder draw border around the layout
   */
  StackedLayout(std::string_view elementName, Size initialSize, ShowBorder showBorder = ShowBorder::No);

  /**
   * Push a new element container to the last index.
   * @return reference to the newly added stack
   */
  Stack &pushStack();
  /**
   * Remove the last element container. If the layout is empty nothing happens.
   */
  void popStack();
  /**
   * Remove stack at the selected index.
   * @param index index of a stack to be removed
   */
  void removeStack(std::size_t index);
  /**
   * Move src stack to the dst index.
   * Stacks at indices srcIndex< <dstIndex are moved one index to the left.
   * @param srcIndex index of the stack to be moved
   * @param dstIndex index of destination of the stack
   */
  void moveStack(std::size_t srcIndex, std::size_t dstIndex);
  /**
   * Get index of currently active stack.
   * @return index of currently active stack
   */
  [[nodiscard]] std::size_t getCurrentIndex() const;
  /**
   * Set active stack by index.
   * @param index index of a stack to activate
   */
  void setIndex(std::size_t index);

  /**
   * Count of stacks in the layout.
   * @return count of stacks
   */
  [[nodiscard]] std::size_t stackSize() const;

  /**
   * Get currently active stack.
   * @return reference to the currently active stack
   */
  [[nodiscard]] Stack &getCurrentStack();
  /**
   * Get stack at the selected index.
   * @param index index of the stack
   * @return reference to the selected stack
   */
  [[nodiscard]] Stack &getStackAtIndex(std::size_t index);

  /**
   * Set active stack;
   * @param stack stack to activate
   */
  void setStackActive(Stack &stack);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::optional<std::size_t> selectedIndex = std::nullopt;
  std::vector<std::unique_ptr<Stack>> stacks;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_LAYOUTS_STACKEDLAYOUT_H
