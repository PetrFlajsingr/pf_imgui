/**
 * @file Range2D.h
 * @brief Element for 2D range selection.
 * @author Petr Flajšingr
 * @date 22.05.20
 */

#ifndef PF_IMGUI_ELEMENTS_RANGE2D_H
#define PF_IMGUI_ELEMENTS_RANGE2D_H

#include <glm/vec2.hpp>
#include <include/DearWidgets/dear_widgets.h>
#include <pf_common/math/Range.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Allows for selection of a 2D area - axis aligned.
 */
class PF_IMGUI_EXPORT Range2D
    : public ItemElement,
      public Labellable,
      public ValueObservable<math::Range<glm::vec2>>,
      public Resizable,
      public Savable,
      public DragSource<math::Range<glm::vec2>>,
      public DropTarget<math::Range<glm::vec2>>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::FrameBackground, style::ColorOf::Border,
                               style::ColorOf::BorderShadow, style::ColorOf::FrameBackgroundActive>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize> {
 public:
  /**
   * Construct Range2D.
   * @param elementName ID of the element
   * @param label text rendered next to the element
   * @param minRange min allowed value
   * @param maxRange max allowed value
   * @param value starting value
   * @param s size on screen
   * @param persistent enable/disable state saving to disk
   */
  Range2D(const std::string &elementName, const std::string &label, const glm::vec2 &minRange,
          const glm::vec2 &maxRange, const math::Range<glm::vec2> &value = math::Range<glm::vec2>{},
          const Size &s = Size::Auto(), Persistent persistent = Persistent::No);

  /**
   * Get min allowed value.
   * @return min allowed value
   */
  [[nodiscard]] const glm::vec2 &getMin() const;
  /**
   * Set min allowed value.
   * @param minRange min allowed value
   */
  void setMin(const glm::vec2 &minRange);
  /**
   * Get max allowed value.
   * @return max allowed value
   */
  [[nodiscard]] const glm::vec2 &getMax() const;
  /**
   * Set max allowed value.
   * @param maxRange max allowed value
   */
  void setMax(const glm::vec2 &maxRange);

 protected:
  void renderImpl() override;

  void unserialize_impl(const toml::table &src) override;
  [[nodiscard]] toml::table serialize_impl() const override;

 private:
  glm::vec2 minRange;
  glm::vec2 maxRange;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_RANGE2D_H
