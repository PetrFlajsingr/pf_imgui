/**
 * @file Dummy.h
 * @brief Space filler dummy.
 * @author Petr Flajšingr
 * @date 6.5.22
 */

#ifndef IMGUI_EXPERIMENTS_DUMMY_H
#define IMGUI_EXPERIMENTS_DUMMY_H

#include <imgui.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

/**
 * @brief An invisible element which just fills certain space.
 */
class Dummy : public Element, public Resizable {
 public:
  struct Config {
    using Parent = Dummy;
    std::string_view name; /*!< Unique name of the element. */
    Size size;             /*!< Size of the element */
  };
  /**
   * Construct Dummy.
   * @param config construction args @see Dummy::Config
   */
  inline Dummy(Config &&config) : Dummy(std::string{config.name}, config.size) {}
  /**
   * Construct Dummy.
   * @param name unique name of the element
   * @param s size of the element
   */
  inline Dummy(const std::string &name, Size s) : Element(name), Resizable(s) {}

 protected:
  inline void renderImpl() override {
    auto renderSize = static_cast<ImVec2>(getSize());
    const auto availableSpace = ImGui::GetContentRegionAvail();
    if (renderSize.x < 0) { renderSize.x += availableSpace.x; }
    if (renderSize.y < 0) { renderSize.y += availableSpace.y; }
    ImGui::Dummy(renderSize);
  }
};

}  // namespace pf::ui::ig

#endif  //IMGUI_EXPERIMENTS_DUMMY_H