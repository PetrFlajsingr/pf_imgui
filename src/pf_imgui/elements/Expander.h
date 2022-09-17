/**
 * @file Expander.h
 * @brief Expander element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_GROUP_H
#define PF_IMGUI_ELEMENTS_GROUP_H

#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Group of elements which is visually delimited with a header.
 *
 * Elements are rendered in top-down order.
 */
class PF_IMGUI_EXPORT Expander : public ItemElement, public ElementContainer, public Savable {
 public:
  /**
   * @brief Struct for construction of Expander.
   */
  struct Config {
    using Parent = Expander;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered in the header of the Expander */
    bool persistent = false;          /*!< Enable/disable state saving */
  };
  /**
   * Construct Expander
   * @param config construction args @see Expander::Config
   */
  explicit Expander(Config &&config);
  /**
   * Construct Expander.
   * @param elementName ID of the group
   * @param labelText text drawn on top of the Expander
   * @param persistent enable/disable state saving
   */
  Expander(std::string_view elementName, std::string_view labelText, Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Property<Label> label;
  Property<bool> collapsed;

 protected:
  void renderImpl() override;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_GROUP_H
