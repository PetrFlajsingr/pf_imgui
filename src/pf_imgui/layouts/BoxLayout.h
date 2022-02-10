/**
 * @file BoxLayout.h
 * @brief Layout boxing children in desired direction.
 * @author Petr Flajšingr
 * @date 24.1.21
 */
#ifndef PF_IMGUI_LAYOUTS_BOXLAYOUT_H
#define PF_IMGUI_LAYOUTS_BOXLAYOUT_H

#include "ResizableLayout.h"
#include <memory>
#include <pf_common/algorithms.h>
#include <pf_common/enums.h>
#include <pf_common/exceptions/Exceptions.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/exceptions.h>
#include <pf_imgui/interface/Resizable.h>
#include <range/v3/view/addressof.hpp>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A layout which positions it's children from left to right or top to bottom, based on the settings.
 *
 * All children are positioned in the way they are added or inserted.
 */
class PF_IMGUI_EXPORT BoxLayout : public ResizableLayout {
 public:
  /**
   * Construct BoxLayout.
   * @param elementName ID of the layout
   * @param layoutDirection direction of children stacking
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param showBorder draw layouts border
   * @param persistent allow state saving
   */
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
            AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
            Persistent persistent = Persistent::No);
  /**
   * Construct BoxLayout.
   * @param elementName ID of the layout
   * @param layoutDirection direction of children stacking
   * @param size size of the layout
   * @param showBorder draw layouts border
   * @param persistent allow state saving
   */
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size, ShowBorder showBorder,
            Persistent persistent = Persistent::No);
  /**
   * Construct BoxLayout.
   * @param elementName ID of the layout
   * @param layoutDirection direction of children stacking
   * @param size size of the layout
   * @param allowCollapse enable collapse button
   * @param persistent allow state saving
   */
  BoxLayout(const std::string &elementName, LayoutDirection layoutDirection, const Size &size,
            AllowCollapse allowCollapse, Persistent persistent = Persistent::No);

  /**
   * Get current layout direction.
   * @return current layout direction
   */
  [[nodiscard]] LayoutDirection getLayoutDirection() const;
  /**
   * Set new layout direction.
   * @param layoutDirection new layout direction
   */
  void setLayoutDirection(LayoutDirection layoutDirection);

  /**
 * Get all children of the layout as references.
 * @return view to references of children in the layout
 */
  [[nodiscard]] inline auto getChildren() {
    return children | ranges::views::transform([](auto &child) -> Element & { return *child; });
  }
  /**
   * Get all children of the layout as references.
   * @return view to const references of children in the layout
   */
  [[nodiscard]] inline auto getChildren() const {
    return children | ranges::views::transform([](auto &child) -> const Element & { return *child; });
  }

  std::vector<Renderable *> getRenderables() override;

  /**
   * Push child to the end.
   * @param child child to be added
   */
  void pushChild(std::unique_ptr<Element> child);

  /**
   * Insert child to the provided index.
   * @param child child to be added
   * @param index
   * @throws InvalidArgumentException if the index is out of bounds
   */
  void insertChild(std::unique_ptr<Element> child, std::size_t index);

  /**
   * Remove child by ID.
   * @param childName ID to be removed
   */
  void removeChild(const std::string &childName);

  /**
    * Create a child and append it to the end of children.
    *
    * @tparam T type of created Element
    * @tparam Args arguments to pass to the Ts constructor after its name
    * @param name ID of the newly created element
    * @param args arguments to pass to the Ts constructor after its nam
    * @return reference to the newly created Element
    *
    * @remark Duplicate check is disabled for MSVC
    * @throws DuplicateIdException when an ID is already present in the container
    */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::constructible_from<T, Args...> T &createChild(Args &&...args) {
#ifndef _MSC_VER  // disabled because of C3779 error
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
#endif
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    pushChild(std::move(child));
    return *ptr;
  }

  template<typename T>
  requires requires { typename T::Parent; }
  std::derived_from<Element> auto &createChild(T &&config) requires(
      std::derived_from<typename T::Parent, Element> &&std::constructible_from<typename T::Parent, T>) {
    auto child = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto ptr = child.get();
    pushChild(std::move(child));
    return *ptr;
  }
  /**
    * Create a child and append it to the end of children.
    *
    * @tparam T type of created Element
    * @tparam Args arguments to pass to the Ts constructor after its name
    * @param index
    * @param name ID of the newly created element
    * @param args arguments to pass to the Ts constructor after its nam
    * @return reference to the newly created Element
    *
    * @remark Duplicate check is disabled for MSVC
    *
    * @throws DuplicateIdException when an ID is already present in the container
    */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::constructible_from<T, Args...> T &createChildAtIndex(std::size_t index,
                                                                                                      Args &&...args) {
#ifndef _MSC_VER  // disabled because of C3779 error
    if (findIf(getChildren() | ranges::views::addressof, [name](const auto &child) {
          return child->getName() == name;
        }).has_value()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
#endif
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

  template<typename T>
  requires requires { typename T::Parent; }
  std::derived_from<Element> auto &createChildAtIndex(std::size_t index, T &&config) requires(
      std::derived_from<typename T::Parent, Element> &&std::constructible_from<typename T::Parent, T>) {
#ifndef _MSC_VER  // TODO: MSVC internal error
    if (const auto iter = children.find(name); iter != children.end()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
#endif
    auto child = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

 protected : void renderImpl() override;
 private:
  LayoutDirection layoutDirection;
  std::vector<std::unique_ptr<Element>> children;

  void renderTopToBottom();
  void renderLeftToRight();
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_LAYOUTS_BOXLAYOUT_H
