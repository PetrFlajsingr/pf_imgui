/**
 * @file RadioGroup.h
 * @brief Radio group element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_RADIOGROUP_H
#define PF_IMGUI_ELEMENTS_RADIOGROUP_H

#include "RadioButton.h"
#include <memory>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Savable.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Group of RadioButton.
 *
 * Only one button in a group can be selected at one time.
 *
 * The selected RadioButton is observable - value is nullptr if no button is selected.
 *
 * Only create this through ImGuiInterface::CreateRadioGroup
 */
class PF_IMGUI_EXPORT RadioGroup : public ValueContainer<RadioButton *, ReadOnlyTag>, public Savable {
 public:
  /**
   * @brief Struct for construction of RadioGroup.
   */
  struct Config {
    using Parent = RadioGroup;
    Explicit<std::string_view> groupName; /*!< Unique name of the element */
    std::vector<RadioButton *> buttons{}; /*!< Buttons inside the group */
    bool persistent = false;              /*!< Enable disk state saving */
  };
  /**
   * Construct RadioGroup
   * @param config construction args @see RadioGroup::Config
   */
  explicit RadioGroup(Config &&config);
  ~RadioGroup() override;
  /**
   * Construct RadioGroup.
   * @param name ID of the element
   * @param childButtons buttons in the group
   * @param persistent enable state saving to disk
   */
  explicit RadioGroup(const std::string &name, std::vector<RadioButton *> childButtons = {},
                      Persistent persistent = Persistent::No);

  /**
   * Create a new button and add it to the group.
   * @param button button to add
   */
  void addButton(RadioButton &button);

  /**
   * Call this on each frame to perform group checks.
   */
  void frame();

  [[nodiscard]] const std::string &getGroupName() const;

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ObservableProperty<RadioGroup, RadioButton *> activeButton;

  [[nodiscard]] RadioButton *const &getValue() const override;

 protected:
  Subscription addValueListenerImpl(std::function<void(RadioButton *const &)> listener) override;

 private:
  void addDestroyListener(RadioButton *button);
  std::string groupName;
  std::vector<RadioButton *> buttons;
  std::vector<Subscription> destroyButtonSubscriptions;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_RADIOGROUP_H
