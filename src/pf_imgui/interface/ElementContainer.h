//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H
#define PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/exceptions.h>
#include <range/v3/view/transform.hpp>
#include <src/pf_imgui/interface/Element.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Interface for objects, which can contain instances of Element.
 *
 * Allows for creation of child elements directly via templated methods.
 *
 * @todo provide std::ranges::forward_range support?
 */
class PF_IMGUI_EXPORT ElementContainer {
 public:
  ElementContainer() = default;

  ElementContainer(ElementContainer &&other) noexcept;
  ElementContainer &operator=(ElementContainer &&other) noexcept;

  /**
   * Create a child and append it to the end of children.
   *
   * @throws DuplicateIdException when an ID is already present in the container
   * @tparam T type of created Element
   * @tparam Args arguments to pass to the Ts constructor after its name
   * @param name ID of the newly created element
   * @param args arguments to pass to the Ts constructor after its nam
   * @return reference to the newly created Element
   */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::constructible_from<T, std::string, Args...> T &
  createChild(std::string name, Args &&...args) {
    if (const auto iter = children.find(name); iter != children.end()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
    const auto ptr = child.get();
    addChild(std::move(child));
    return *ptr;
  }

  /**
  * Create a child and append it to the end of children.
  *
  * @tparam T type of created Element
  * @tparam Args arguments to pass to the Ts constructor after its name
  * @param index index to insert the newly created child at
  * @param name ID of the newly created element
  * @param args arguments to pass to the Ts constructor after its nam
  * @return reference to the newly created Element
  *
  * @throws DuplicateIdException when an ID is already present in the container
  * @throws InvalidArgumentException when index is out of bounds
  */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::constructible_from<T, std::string, Args...> T &
  createChildAtIndex(std::size_t index, std::string name, Args &&...args) {
    if (const auto iter = children.find(name); iter != children.end()) {
      throw DuplicateIdException("{} already present in ui", name);
    }
    auto child = std::make_unique<T>(name, std::forward<Args>(args)...);
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
   *
   * @throws InvalidArgumentException when index is out of bounds
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
   * @tparam T type of the searched for child. Element can be used to assure no invalid cast occurs.
   * @param name name of the searched for child
   * @return reference to the searched for child
   *
   * @throws InvalidArgumentException when the child is not found or when it doesn't match the desired type
   */
  template<std::derived_from<Element> T>
  [[nodiscard]] T &childByName(const std::string &name) {
    if (const auto iter = children.find(name); iter != children.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(iter->second); result != nullptr) { return result; }
      throw InvalidArgumentException::fmt("Wrong type for child: '{}'", name);
    }
    throw InvalidArgumentException::fmt("Child not found: '{}'", name);
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
  [[nodiscard]] auto getChildren() const {
    return childrenInOrder | ranges::views::transform([](auto &childRef) -> const Element & { return childRef.get(); });
  }

  /**
   * Remove all children from the container.
   */
  void clear();

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

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_INTERFACE_CONTAINER_H
