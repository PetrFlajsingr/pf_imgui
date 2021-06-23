/**
 * @file AnchorLayout.h
 * @brief Layout where each element's size or position is automatically changed based on anchors.
 * @author Petr Flajšingr
 * @date 13.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H

#include "ResizableLayout.h"
#include <pf_common/algorithms.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/exceptions.h>
#include <pf_imgui/interface/decorators/PositionDecorator.h>
#include <pf_imgui/interface/decorators/WidthDecorator.h>
#include <range/v3/view/addressof.hpp>
#include <range/v3/view/transform.hpp>
#include <tuple>

namespace pf::ui::ig {

/**
 * @brief Layout containing elements which can be placed in custom location and their size is changed based on anchors.
 *
 * @todo: combine this with absolute layout
 * @todo: layouts can't see they were resized
 */
class PF_IMGUI_EXPORT AnchorLayout : public ResizableLayout {
 public:
  /**
   * Construct AnchorLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  AnchorLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse = AllowCollapse::No,
               ShowBorder showBorder = ShowBorder::No, Persistent persistent = Persistent::No);
  /**
   * Construct AnchorLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  AnchorLayout(const std::string &elementName, const Size &size, ShowBorder showBorder,
               Persistent persistent = Persistent::No);
  /**
   * Construct AnchorLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param persistent enable state saving
   */
  AnchorLayout(const std::string &elementName, const Size &size, AllowCollapse allowCollapse,
               Persistent persistent = Persistent::No);
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
  auto &createChild(const std::string &name, ImVec2 position, Flags<Anchor> anchors, Args &&...args) {
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
    constexpr auto IsPositionable = std::derived_from<T, Positionable>;
    using CreateType = std::conditional_t<IsPositionable, T, PositionDecorator<T>>;
    auto child = std::make_unique<CreateType>(position, name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    std::function<void(float)> addToHeight = [](float) {};
    std::function<void(float)> addToWidth = [](float) {};
    if constexpr (std::derived_from<T, Resizable>) {
      addToWidth = [ptr = child.get()](float d) {
        auto size = ptr->getSize();
        size.width = std::clamp(size.width + d, 0.f, std::numeric_limits<float>::max());
        ptr->setSize(size);
      };
      addToHeight = [ptr = child.get()](float d) {
        auto size = ptr->getSize();
        size.height = std::clamp(size.height + d, 0.f, std::numeric_limits<float>::max());
        ptr->setSize(size);
      };
    } else if constexpr (std::derived_from<T, WidthDecorator<T>>) {
      addToWidth = [ptr = child.get()](float d) {
        const auto width = std::clamp(ptr->getWidth() + d, 0.f, std::numeric_limits<float>::max());
        ptr->setWidth(width);
      };
    }
    children.template emplace_back(std::move(child), dynamic_cast<Positionable *>(ptr), static_cast<Anchor>(*anchors),
                                   addToWidth, addToHeight);
    return *ptr;
  }
  void setSize(const Size &s) override;

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
  inline auto getChildren() const {
    return children | ranges::views::transform([](auto &child) -> const Element & { return *child.element; });
  }

  /**
   * Remove child by ID.
   * If the child is not present nothing happens/
   * @param name ID of the element
   */
  void removeChild(const std::string &name);

  /**
   * Set position by elements' ID.
   * @param name ID of the child
   * @param position new position
   * @throws IdNotFoundException when the child is not present in the layout
   */
  void setChildPosition(const std::string &name, ImVec2 position);

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  struct AnchoredChild {
    std::unique_ptr<Element> element;
    Positionable *positionable;
    Flags<Anchor> anchors;
    std::function<void(float)> addToWidth;
    std::function<void(float)> addToHeight;
  };
  std::vector<AnchoredChild> children;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_LAYOUTS_ANCHORLAYOUT_H
