/**
 * @file AnchorLayout.h
 * @brief Layout where each element's size or position is automatically changed based on anchors.
 * @author Petr Flajšingr
 * @date 13.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H

#include <limits>
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_common/algorithms.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/concepts/HasSizeObservable.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/decorators/WidthDecorator.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Layout containing elements which can be placed in custom location and their size is changed based on anchors.
 *
 * @todo: layouts can't see they were resized
 */
class PF_IMGUI_EXPORT AnchorLayout : public Layout {
 public:
  /**
   * @brief Struct for construction of AnchorLayout.
   */
  struct Config {
    using Parent = AnchorLayout;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<Size> size;             /*!< Size of the element */
    bool showBorder = false;         /*!< Render border around layout's area */
  };
  /**
   * Construct AnchorLayout
   * @param config construction args @see AnchorLayout::Config
   */
  explicit AnchorLayout(Config &&config);
  /**
   * Construct AnchorLayout.
   * @param elementName ID of the layout
   * @param initialSize size of the layout
   * @param allowCollapse enable collapse button
   * @param showBorder draw border around the layout
   */
  AnchorLayout(const std::string &elementName, const Size &initialSize, ShowBorder showBorder = ShowBorder::No);
  /**
    * Create a child and append it to the end of children.
    *
    * @tparam T type of created ElementWithID
    * @tparam Args arguments to pass to the Ts constructor after its name
    * @param name ID of the newly created element
    * @param position position of the newly created element
    * @param args arguments to pass to the Ts constructor after its nam
    * @return reference to the newly created ElementWithID
    */
  template<typename T, typename... Args>
    requires std::derived_from<T, Element> && std::constructible_from<T, Args...>
  auto &createChild(Position position, const Flags<Anchor> &anchors, Args &&...args) {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    std::function<void(Height)> addToHeight = [](Height) {};
    std::function<void(Width)> addToWidth = [](Width) {};
    if constexpr (HasSizeObservable<T>) {
      addToWidth = [ptr = child.get()](Width d) {
        auto childSize = *ptr->size;
        childSize.width = std::clamp(static_cast<float>(childSize.width) + static_cast<float>(d), 0.1f,
                                     std::numeric_limits<float>::max());
        *ptr->size.modify() = childSize;
      };
      addToHeight = [ptr = child.get()](Height d) {
        auto childSize = *ptr->size;
        childSize.height = childSize.height + d;
        if (childSize.height < Height{0}) { childSize.height = 0.1f; }
        *ptr->size.modify() = childSize;
      };
    } else if constexpr (std::derived_from<T, WidthDecorator<T>>) {
      addToWidth = [ptr = child.get()](Width d) {
        auto childWidth = ptr->getWidth() + d;
        if (childWidth < Width{0}) { childWidth = 0.1f; }
        ptr->setWidth(childWidth);
      };
    }
    children.emplace_back(std::move(child), position, static_cast<Anchor>(*anchors), addToWidth, addToHeight);
    return *ptr;
  }

  /**
   * Get all children of the layout as references.
   * @return view to references of children in the layout
   */
  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child.element; });
  }
  /**
   * Get all children of the layout as references.
   * @return view to const references of children in the layout
   */
  [[nodiscard]] inline auto getChildren() const {
    return children | ranges::views::transform([](auto &child) -> const Element & { return *child.element; });
  }

  /**
   * Remove child by ID.
   * If the child is not present nothing happens/
   * @param childName ID of the element
   */
  void removeChild(const std::string &childName);

  /**
   * Set position by elements' ID.
   * @param childName ID of the child
   * @param position new position
   */
  void setChildPosition(const std::string &childName, Position position);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  void onSizeUpdated(Size previousSize);

  struct AnchoredChild {
    std::unique_ptr<Element> element;
    Position position;
    Flags<Anchor> anchors;
    std::function<void(Width)> addToWidth;
    std::function<void(Height)> addToHeight;
  };
  std::vector<AnchoredChild> children;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H
