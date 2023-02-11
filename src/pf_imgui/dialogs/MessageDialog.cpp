//
// Created by xflajs00 on 06.05.2022.
//

#include "MessageDialog.h"
#include <pf_imgui/elements/Button.h>
#include <pf_imgui/elements/Dummy.h>
#include <pf_imgui/elements/Text.h>
#include <pf_imgui/layouts/HorizontalLayout.h>

namespace pf::ui::ig {

MessageDialog::MessageDialog(std::string_view elementName, std::string_view title, Size initialSize, std::string_view message,
                             const Flags<MessageButtons> &buttons)
    : ModalDialog(elementName, title, initialSize) {
  createChild<Text>("text", std::string{message}).setWrap(true);
  createChild<Dummy>("area_fill", Size{Width::Auto(), Height::Fill(25)});
  auto &btnLayout = createChild<HorizontalLayout>("hor_layout", Size{Width::Auto(), 20}, HorizontalAlign::Down);
  auto enabledButtons = buttons.getSetFlags();
  std::ranges::for_each(enabledButtons, [this, &btnLayout](MessageButtons buttonType) {
    btnLayout.createChild<Button>("button" + std::to_string(static_cast<int>(buttonType)), std::string{magic_enum::enum_name(buttonType)})
        .clickEvent.addListener([this, buttonType] {
          if (dialogDone(buttonType)) { close(); }
        });
  });
}

}  // namespace pf::ui::ig