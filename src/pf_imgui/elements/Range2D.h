/**
 * @file Range2D.h
 * @brief Element for 2D range selection.
 * @author Petr Flajšingr
 * @date 22.05.20
 */

#ifndef PF_IMGUI_ELEMENTS_RANGE2D_H
#define PF_IMGUI_ELEMENTS_RANGE2D_H

#include <dear_widgets.h>
#include <glm/vec2.hpp>
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
   * @brief Struct for construction of Range2D.
   */
  struct Config {
    using Parent = Range2D;
    std::string_view name;          /*!< Unique name of the element */
    std::string_view label;         /*!< Text rendered next to the element */
    glm::vec2 min;                  /*!< Minimum allowed value */
    glm::vec2 max;                  /*!< Maximum allowed value*/
    math::Range<glm::vec2> value{}; /*!< Initial value */
    Size size = Size::Auto();       /*!< Size of the element */
    bool persistent = false;        /*!< Allow state saving to disk */
  };
  /**
   * Construct Range2D
   * @param config construction args @see Range2D::Config
   */
  explicit Range2D(Config &&config);
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

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  glm::vec2 minRange;
  glm::vec2 maxRange;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_RANGE2D_H
