/**
 * @file AbsoluteLayout.h
 * @brief Layout allowing positioning children with absolute position.
 * @author Petr Flajšingr
 * @date 25.1.21
 */

#ifndef PF_IMGUI_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_LAYOUTS_ABSOLUTELAYOUT_H

#include <memory>
#include <pf_common/algorithms.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Layout.h>
#include <pf_imgui/interface/decorators/PositionDecorator.h>
#include <pf_imgui/meta.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <utility>
#include <vector>
#include <pf_common/Explicit.h>

namespace pf::ui::ig {

/**
 * @brief A layout which positions it's children in their given coordinates.
 *
 * All children are wrapped int a PositionDecorator, which allows for settings position even to not Positionable elements.
 * Coordinates are the same as the ones used for size.
 */
class PF_IMGUI_EXPORT AbsoluteLayout : public Layout {
 public:
  /**
   * @brief Struct for construction of AbsoluteLayout.
   */
  struct Config {
    using Parent = AbsoluteLayout;
    Explicit<std::string_view> name;   /*!< Unique name of the element */
    Explicit<Size> size;               /*!< Size of the element */
    bool showBorder = false; /*!< Render border around layout's area */
  };
  /**
   * Construct AbsoluteLayout
   * @param config construction args @see AbsoluteLayout::Config
   */
  explicit AbsoluteLayout(Config &&config);
  /**
   * Construct AbsoluteLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  AbsoluteLayout(const std::string &elementName, const Size &size, ShowBorder showBorder = ShowBorder::No);

  /**
   * Get all children of the layout as references.
   * @return view to references of children in the layout
   */
  auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child.first; });
  }
  /**
   * Get all children of the layout as references.
   * @return view to const references of children in the layout
   */
  [[nodiscard]] auto getChildren() const {
    return children | ranges::views::transform([](auto &child) -> const Element & { return *child.first; });
  }

  /**
    * Create a child and append it to the end of children.
    *
    * @tparam T type of created Element
    * @tparam Args arguments to pass to the Ts constructor after its name
    * @param name ID of the newly created element
    * @param position position of the newly created element
    * @param args arguments to pass to the Ts constructor after its nam
    * @return reference to the newly created Element
    */
  template<typename T, typename... Args>
    requires std::derived_from<T, Element> && std::constructible_from<T, Args...>
  auto &createChild(ImVec2 position, Args &&...args) {
    constexpr auto IsPositionable = std::derived_from<T, Positionable>;
    using CreateType = std::conditional_t<IsPositionable, T, PositionDecorator<T>>;
    auto child = std::make_unique<CreateType>(position, std::forward<Args>(args)...);
    const auto ptr = child.get();
    children.emplace_back(std::move(child), dynamic_cast<Positionable *>(ptr));
    return *ptr;
  }

  template<ElementConstructConfig T>
  typename T::Parent &createChild(ImVec2 position, T &&config) {
    constexpr auto IsPositionable = std::derived_from<typename T::Parent, Positionable>;
    using CreateType = std::conditional_t<IsPositionable, typename T::Parent, PositionDecorator<typename T::Parent>>;
    auto child = std::make_unique<CreateType>(position, std::forward<T>(config));
    const auto ptr = child.get();
    children.emplace_back(std::move(child), dynamic_cast<Positionable *>(ptr));
    return *ptr;
  }

  /**
   * Set position by elements' ID.
   * @param childName ID of the child
   * @param position new position
   */
  void setChildPosition(const std::string &childName, Position position);

  /**
   * Remove child by ID.
   * If the child is not present nothing happens/
   * @param childName ID of the element
   */
  void removeChild(const std::string &childName);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::pair<std::unique_ptr<Element>, Positionable *>> children;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_LAYOUTS_ABSOLUTELAYOUT_H
