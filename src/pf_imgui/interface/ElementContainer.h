/**
 * @file ElementContainer.h
 * @brief Base class for objects which can hold arbitrary elements.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_INTERFACE_CONTAINER_H
#define PF_IMGUI_INTERFACE_CONTAINER_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/RenderablesContainer.h>
#include <pf_imgui/meta.h>
#include <range/v3/view/transform.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Interface for objects, which can contain instances of ElementWithID.
 *
 * Allows for creation of child elements directly via templated methods.
 */
class PF_IMGUI_EXPORT ElementContainer : public RenderablesContainer {
 public:
  ElementContainer() = default;

  ElementContainer(ElementContainer &&other) noexcept;
  ElementContainer &operator=(ElementContainer &&other) noexcept;

  /**
   * Create a child and append it to the end of children.
   *
   * @tparam T type of created ElementWithID
   * @tparam Args arguments to pass to the Ts constructor after its name
   * @param args arguments to pass to the Ts constructor after its nam
   * @return reference to the newly created ElementWithID
   */
  template<typename T, typename... Args>
    requires std::derived_from<T, Element> && std::constructible_from<T, Args...>
  T &createChild(Args &&...args) {
    auto child = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = child.get();
    addChild(std::move(child));
    return *ptr;
  }

  template<ElementConstructConfig T>
  std::derived_from<Element> auto &createChild(T &&config) {
    auto child = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto ptr = child.get();
    addChild(std::move(child));
    return *ptr;
  }

  /**
   * Create a child and append it to the end of children.
   *
   * @tparam T type of created ElementWithID
   * @tparam Args arguments to pass to the Ts constructor after its name
   * @param index index to insert the newly created child at
   * @param name ID of the newly created element
   * @param args arguments to pass to the Ts constructor after its nam
   * @return reference to the newly created ElementWithID
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
  std::derived_from<Element> auto &createChildAtIndex(std::size_t index, T &&config) {
    auto child = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto ptr = child.get();
    insertChild(std::move(child), index);
    return *ptr;
  }

  /**
   * Remove a child by name.
   *
   * @details If the container doesn't contain any child by this name, nothing is done.
   * @param name name of the child to be removed
   */
  void removeChild(const std::string &name);
  /**
   * Add an already created child.
   *
   * @param child child to be added
   */
  void addChild(std::unique_ptr<Element> child);
  /**
   * Insert an already created child at the provided index.
   *
   * @param child child to be added
   * @param index index at which the child should be stored
   */
  void insertChild(std::unique_ptr<Element> child, std::size_t index);

  /**
   * Enqueue child removal by its name. The removal is run in the next call to getChildren().
   * @param name name of the child to be removed
   */
  void enqueueChildRemoval(const std::string &name);

  /**
   * Find a child by its name.
   *
   * @tparam T type of the searched for child. ElementWithID can be used to assure no invalid cast occurs.
   * @param name name of the searched for child
   * @return reference to the searched for child
   */
  template<std::derived_from<Element> T>
  [[nodiscard]] std::optional<std::reference_wrapper<T>> childByName(const std::string &name) {
    if (const auto iter = children.find(name); iter != children.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(iter->second); result != nullptr) { return result; }
      return std::nullopt;
    }
    return std::nullopt;
  }

  /**
   * Find a child by its name.
   *
   * @tparam T type of the searched for child. ElementWithID can be used to assure no invalid cast occurs.
   * @param name name of the searched for child
   * @return reference to the searched for child
   */
  template<std::derived_from<Element> T>
  [[nodiscard]] std::optional<std::reference_wrapper<const T>> childByName(const std::string &name) const {
    if (const auto iter = children.find(name); iter != children.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(iter->second); result != nullptr) { return result; }
      return std::nullopt;
    }
    return std::nullopt;
  }

  /**
   * Get all children.
   *
   * @details Upon calling this method, all children enqueued for removal are removed first.
   * @return View to references of all children in the container.
   */
  [[nodiscard]] auto getChildren() {
    std::ranges::for_each(childrenToRemove, [this](const auto &name) { removeChild(name); });
    childrenToRemove.clear();
    return childrenInOrder | ranges::views::transform([](auto &childRef) -> Element & { return childRef.get(); });
  }
  /**
   * Get all children.
   *
   * @details Unlike it's non-const alternative, this method does not remove children enqueued for removal.
   * @return View to const references of all children in the container.
   */
  [[nodiscard]] auto getChildren() const {  //-V659
    return childrenInOrder | ranges::views::transform([](auto &childRef) -> const Element & { return childRef.get(); });
  }

  /**
   * Remove all children from the container.
   */
  void clear();

  std::vector<Renderable *> getRenderables() override;

 private:
  /**
   * Main storage of children. Stored as an unordered_map for quick access by ID.
   */
  std::unordered_map<std::string, std::unique_ptr<Element>> children;
  /**
   * Secondary storage, where children are stored in their insertion order.
   */
  std::vector<std::reference_wrapper<Element>> childrenInOrder;
  /**
   * Storage for IDs of children which should be removed during the next getChildren() call.
   */
  std::vector<std::string> childrenToRemove;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_CONTAINER_H
