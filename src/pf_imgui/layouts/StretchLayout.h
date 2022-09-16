/**
 * @file StretchLayout.h
 * @brief Layout which stretches its child in desired direction.
 * @author Petr Flajšingr
 * @date 25.1.21
 */
#ifndef PF_IMGUI_LAYOUTS_STRETCHLAYOUT_H
#define PF_IMGUI_LAYOUTS_STRETCHLAYOUT_H

#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/concepts/ConfigConstruction.h>
#include <pf_imgui/concepts/HasSizeObservable.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/Layout.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {
/**
 * @brief A layout which stretches to the available area in desired direction.
 *
 *
 */
class PF_IMGUI_EXPORT StretchLayout : public Layout {
 public:
  /**
   * @brief Struct for construction of StretchLayout.
   */
  struct Config {
    using Parent = StretchLayout;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<Size> size;             /*!< Size of the element */
    Explicit<Stretch> stretch;       /*!< Stretch direction */
    bool showBorder = false;         /*!< Render border around layout's area */
  };
  /**
   * Construct StretchLayout
   * @param config construction args @see StretchLayout::Config
   */
  explicit StretchLayout(Config &&config);
  /**
   * Construct StretchLayout.
   * @param elementName ID of the layout
   * @param initialSize size of the layout
   * @param stretchType dimension to which the layout stretches
   * @param showBorder draw border around the layout
   */
  StretchLayout(std::string_view elementName, Size initialSize, Stretch stretchType,
                ShowBorder showBorder = ShowBorder::No);

  /**
   * Create a child inside the layout. If the layout already contains one it gets overwritten.
   *
   * @tparam T type of created ElementWithID
   * @tparam Args arguments to pass to the Ts constructor after its name
   * @param name ID of the newly created element
   * @param args arguments to pass to the Ts constructor after its nam
   * @return reference to the newly created ElementWithID
   *
   */
  template<std::derived_from<Element> T, typename... Args>
    requires HasSizeObservable<T> && std::constructible_from<T, Args...>
  T &createChild(Args &&...args) {
    auto origChild = std::make_unique<T>(std::forward<Args>(args)...);
    auto childPtr = origChild.get();
    setChild(std::move(origChild));
    return *childPtr;
  }

  template<ElementConstructConfig T>
    requires HasSizeObservable<typename T::Parent>
  std::derived_from<Element> auto &createChild(T &&config) {
    auto origChild = std::make_unique<typename T::Parent>(std::forward<T>(config));
    const auto childPtr = origChild.get();
    setChild(std::move(origChild));
    return *childPtr;
  }

  /**
   * Set child inside the layout. If the layout already contains one it gets overwritten.
   * @param newChild
   */
  template<std::derived_from<Element> T>
    requires HasSizeObservable<T>
  void setChild(std::unique_ptr<T> &&newChild) {
    updateChildSize = [this, &childSize = newChild->size](Size newSize) { *childSize.modify() = newSize; };
    child = std::move(newChild);
  }

  std::vector<Renderable *> getRenderables() override;

  /**
   * Get child of the layout.
   * @return reference to the child
   */
  [[nodiscard]] std::optional<std::reference_wrapper<Element>> getChild();
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
  std::unique_ptr<Element> child = nullptr;
  std::function<void(Size)> updateChildSize = nullptr;
  ImVec2 previousSize;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_LAYOUTS_STRETCHLAYOUT_H
