/**
* @file HorizontalLayout.h
* @brief Layout rendering its children horizontally.
* @author Petr Flajšingr
* @date 10.4.22
*/

#ifndef IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H
#define IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H

#include "ResizableLayout.h"
#include <memory>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/meta.h>
#include <range/v3/view/transform.hpp>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Layout rendering its children horizontal with optional spacing.
 */
class PF_IMGUI_EXPORT HorizontalLayout : public ResizableLayout {
 public:
  /**
   * @brief Struct for construction of HorizontalLayout.
   */
  struct Config {
    using Parent = HorizontalLayout;
    std::string_view name;                  /*!< Unique name of the element */
    Size size;                              /*!< Size of the element */
    float spacing = 0.f;                    /*!< Spaces between elements */
    ShowBorder showBorder = ShowBorder::No; /*!< Render border around layout's area */
  };
  /**
   * Config constructor of HorizontalLayout @see HorizontalLayout::Config
   * @param config construction args
   */
  explicit HorizontalLayout(Config &&config);

  /**
   * Construct HorizontalLayout
   * @param name unique name of the element
   * @param size size of the layout
   * @param elementSpacing spaces between elements
   * @param showBorder render border around layout's area
   */
  HorizontalLayout(const std::string &name, Size size, float elementSpacing = 0.f,
                   ShowBorder showBorder = ShowBorder::No);

  /**
   *
   * @return current spacing between elements
   */
  [[nodiscard]] float getSpacing() const;
  /**
   * Set spacing between elements
   * @param newSpacing
   */
  void setSpacing(float newSpacing);

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
  float spacing;

  std::vector<std::unique_ptr<Element>> children;
};
}  // namespace pf::ui::ig

#endif  //IMGUI_EXPERIMENTS_HORIZONTALLAYOUT_H
