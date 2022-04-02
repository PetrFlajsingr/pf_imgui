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
#include <pf_imgui/meta.h>
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
   * @brief Struct for construction of BoxLayout.
   */
  struct Config {
    using Parent = BoxLayout;
    std::string_view name;                           /*!< Unique name of the element */
    LayoutDirection layoutDirection;                 /*!< Direction the element are rendered in */
    Size size;                                       /*!< Size of the element */
    AllowCollapse allowCollapse = AllowCollapse::No; /*!< Allow collapse functionality */
    ShowBorder showBorder = ShowBorder::No;          /*!< Render border around layout's area */
    Persistent persistent = Persistent::No;          /*!< Allow state saving to disk */
  };
  /**
   * Construct BoxLayout
   * @param config construction args @see BoxLayout::Config
   */
  explicit BoxLayout(Config &&config);
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
    */
  template<typename T, typename... Args>
    requires std::derived_from<T, Element> && std::constructible_from<T, Args...>
  T &createChild(Args &&...args) {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    pushChild(std::move(child));
    return *ptr;
  }

  template<ElementConstructConfig T>
  typename T::Parent &createChild(T &&config) {
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
    */
  template<typename T, typename... Args>
    requires std::derived_from<T, Element> && std::constructible_from<T, Args...>
  T &createChildAtIndex(std::size_t index, Args &&...args) {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

  template<ElementConstructConfig T>
  typename T::Parent &createChildAtIndex(std::size_t index, T &&config) {
    auto child = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

 protected:
  void renderImpl() override;

 private:
  LayoutDirection layoutDirection;
  std::vector<std::unique_ptr<Element>> children;

  void renderTopToBottom();
  void renderLeftToRight();
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_LAYOUTS_BOXLAYOUT_H
