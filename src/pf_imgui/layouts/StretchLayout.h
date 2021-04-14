//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H

#include "ResizableLayout.h"
#include <memory>
#include <pf_imgui/enums.h>

namespace pf::ui::ig {
/**
 * @brief A layout which stretches to the available area in desired direction.
 *
 *
 */
class StretchLayout : public ResizableLayout {
 public:
  /**
   * Construct StretchLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param stretch dimension to which the layout stretches
   * @param allowCollapse enable collapse button
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch,
                AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
                Persistent persistent = Persistent::No);
  /**
   * Construct StretchLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param stretch dimension to which the layout stretches
   * @param showBorder draw border around the layout
   * @param persistent enable state saving
   */
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch, ShowBorder showBorder,
                Persistent persistent = Persistent::No);
  /**
   * Construct StretchLayout.
   * @param elementName ID of the layout
   * @param size size of the layout
   * @param stretch dimension to which the layout stretches
   * @param allowCollapse enable collapse button
   * @param persistent enable state saving
   */
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch, AllowCollapse allowCollapse,
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

  /**
   * Create a child inside the layout. If the layout already contains one it gets overwritten.
   *
   * @tparam T type of created Element
   * @tparam Args arguments to pass to the Ts constructor after its name
   * @param name ID of the newly created element
   * @param args arguments to pass to the Ts constructor after its nam
   * @return reference to the newly created Element
   *
   */
  template<typename T, typename... Args>
  requires std::derived_from<T, Element> && std::derived_from<T, Resizable> && std::constructible_from<T, std::string,
                                                                                                       Args...>
      T &createChild(std::string name, Args &&...args) {
    child = std::make_unique<T>(name, std::forward<Args>(args)...);
    renderableChild = dynamic_cast<Renderable *>(child.get());
    return *child;
  }

  /**
   * Set child inside the layout. If the layout already contains one it gets overwritten.
   * @param newChild
   */
  template<std::derived_from<Element> T>
  void setChild(std::unique_ptr<T> &&newChild) {
    child = std::move(newChild);
  }

  std::vector<Renderable *> getRenderables() override;

  /**
   * Get child of the layout.
   * @return reference to the child
   * @throws StackTraceException when no child is present inside the layout
   */
  [[nodiscard]] Element &getChild();
  /**
   * Check whether the layout contains a child.
   * @return true if the layout contains a child, false otherwise
   */
  [[nodiscard]] bool hasChild() const;

  /**
   * Get current stretch setting.
   * @return current stretch setting
   */
  [[nodiscard]] Stretch getStretch() const;
  /**
   * Set new stretch setting.
   * @param newStretch new stretch setting
   */
  void setStretch(Stretch newStretch);

 protected:
  void renderImpl() override;

 private:
  Stretch stretch;
  std::unique_ptr<Resizable> child = nullptr;
  Renderable *renderableChild;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H
