//
// Created by xflajs00 on 06.05.2022.
//

#include "InputDialog.h"
#include <pf_imgui/elements/Button.h>
#include <pf_imgui/elements/Dummy.h>
#include <pf_imgui/elements/InputText.h>
#include <pf_imgui/elements/Text.h>
#include <pf_imgui/layouts/HorizontalLayout.h>

namespace pf::ui::ig {

InputDialog::InputDialog(std::string_view elementName, std::string_view title, Size s, std::string_view message)
    : ModalDialog(elementName, title, s) {
  createChild<Text>("text", std::string{message});
  auto &input = createChild<InputText>("input", "", "");
  createChild<Dummy>("area_fill", Size{Width::Auto(), Height::Fill(25)});
  auto &btnLayout = createChild<HorizontalLayout>("hor_layout", Size{Width::Auto(), Height::Fill(0)});
  btnLayout.createChild<Button>("button_ok", "Ok").clickEvent.addListener([this, &input] {
    inputDone(input.getValue());
    close();
  });
  btnLayout.createChild<Button>("button_cancel", "Cancel").clickEvent.addListener([this] {
    cancelClicked();
    close();
  });
}
}  // namespace pf::ui::ig