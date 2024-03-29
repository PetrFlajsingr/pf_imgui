/**
 * @file Dummy.h
 * @brief Space filler dummy.
 * @author Petr Flajšingr
 * @date 6.5.22
 */

#ifndef PF_IMGUI_ELEMENTS_DUMMY_H
#define PF_IMGUI_ELEMENTS_DUMMY_H

#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/Element.h>

namespace pf::ui::ig {

/**
 * @brief An invisible element which just fills certain space.
 */
class Dummy : public Element {
 public:
  struct Config {
    using Parent = Dummy;
    Explicit<std::string_view> name; /*!< Unique name of the element. */
    Explicit<Size> size;             /*!< Size of the element */
  };
  /**
   * Construct Dummy.
   * @param config construction args @see Dummy::Config
   */
  inline Dummy(Config &&config) : Dummy(config.name, config.size) {}
  /**
   * Construct Dummy.
   * @param elementName unique name of the element
   * @param s size of the element
   */
  inline Dummy(std::string_view elementName, Size s) : Element(elementName), size(s) {}

  Property<Size> size;

 protected:
  inline void renderImpl() override {
    auto renderSize = static_cast<ImVec2>(*size);
    const auto availableSpace = ImGui::GetContentRegionAvail();
    if (renderSize.x < 0) { renderSize.x += availableSpace.x; }
    if (renderSize.y < 0) { renderSize.y += availableSpace.y; }
    ImGui::Dummy(renderSize);
  }
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_DUMMY_H
