//
// Created by xflajs00 on 06.05.2022.
//

#include "InputDialog.h"
#include <pf_imgui/elements/Dummy.h>

namespace pf::ui::ig {

InputDialog::InputDialog(const std::string &elementName, const std::string &title, Size s, const std::string &message)
    : ModalDialog(elementName, title) {
  createChild<Text>("text", message);
  auto &input = createChild<InputText>("input", "", "");
  createChild<Dummy>("area_fill", Size{Width::Auto(), Height::Fill(25)});
  auto &btnLayout = createChild<HorizontalLayout>("hor_layout", Size{Width::Auto(), Height::Fill(0)});
  btnLayout.createChild<Button>("button_ok", "Ok").addClickListener([this, &input] {
    inputDone(input.getValue());
    close();
  });
  btnLayout.createChild<Button>("button_cancel", "Cancel").addClickListener([this] {
    cancelClicked();
    close();
  });
  setSize(s);
}
}  // namespace pf::ui::ig