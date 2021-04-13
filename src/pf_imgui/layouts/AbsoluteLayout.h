//
// Created by xflajs00 on 25.01.2021.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H

#include "BoxLayout.h"
#include "ResizableLayout.h"
#include "layout_decorators.h"
#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/exceptions.h>
#include <pf_imgui/interface/Positionable.h>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A layout which positions it's children in their given coordinates.
 *
 * All children are wrapped int a PositionDecorator, which allows for settings position even to not Positionable elements.
 * Coordinates are the same as the ones used for size.
 */
class PF_IMGUI_EXPORT AbsoluteLayout : public ResizableLayout {
 public:
  /**
   * Construct AbsolutLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  AbsoluteLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse = AllowCollapse::No,
                 ShowBorder showBorder = ShowBorder::No, Persistent persistent = Persistent::No);
  /**
   * Construct AbsolutLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  AbsoluteLayout(const std::string &elementName, const ImVec2 &size, ShowBorder showBorder,
                 Persistent persistent = Persistent::No);
  /**
   * Construct AbsolutLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param persistent enable state saving
   */
  AbsoluteLayout(const std::string &elementName, const ImVec2 &size, AllowCollapse allowCollapse,
                 Persistent persistent = Persistent::No);

  /**
   * Get all children of the layout as references.
   * @return view to references of children in the layout
   */
  inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child.first; });
  }
  /**
   * Get all children of the layout as references.
   * @return view to const references of children in the layout
   */
  inline auto getChildren() const {
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
    *
    * @throws DuplicateIdException when an ID is already present in the container
    */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::constructible_from<T, std::string, Args...>
  auto &createChild(std::string name, ImVec2 position, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
    constexpr auto IsPositionable = std::derived_from<T, Positionable>;
    using CreateType = std::conditional_t<IsPositionable, T, PositionDecorator<T>>;
    auto child = std::make_unique<CreateType>(position, name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    children.template emplace_back(std::move(child), dynamic_cast<Positionable *>(ptr));
    return *ptr;
  }

  /**
   * Set position by elements' ID.
   * @param name ID of the child
   * @param position new position
   * @throws IdNotFoundException when the child is not present in the layout
   */
  void setChildPosition(const std::string &name, ImVec2 position);

  /**
   * Remove child by ID.
   * If the child is not present nothing happens/
   * @param name ID of the element
   */
  void removeChild(const std::string &name);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  std::vector<std::pair<std::unique_ptr<Element>, Positionable *>> children;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_ABSOLUTELAYOUT_H
