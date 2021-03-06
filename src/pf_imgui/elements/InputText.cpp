//
// Created by petr on 10/31/20.
//

#include "InputText.h"
#include <imgui.h>
#include <toml++/toml_node_view.h>
#include <utility>

namespace pf::ui::ig {
InputText::InputText(const std::string &elementName, std::string label, const std::string &text,
                     TextInputType textInputType, Persistent persistent)
    : Text(elementName, text), Labellable(std::move(label)), ValueObservable(""), Savable(persistent),
      inputType(textInputType) {
  setValue(text);
}

void InputText::renderImpl() {
  auto valueChanged = false;
  if (inputType == TextInputType::SingleLine) {
    valueChanged = ImGui::InputText(getLabel().c_str(), buffer, 256);
  } else {
    valueChanged = ImGui::InputTextMultiline(getLabel().c_str(), buffer, 256);
  }
  if (valueChanged && strcmp(buffer, getText().c_str()) != 0) {
    setText(buffer);
    setValue(getText());
    notifyValueChanged();
  }
}
void InputText::clear() {
  setText("");
  buffer[0] = '\0';
}

void InputText::unserialize_impl(const toml::table &src) {
  setText(**src["text"].as_string());
  setValueAndNotifyIfChanged(getText());
  std::snprintf(buffer, getText().size(), "%s", getText().c_str());
}

toml::table InputText::serialize_impl() { return toml::table{{{"text", getText()}}}; }

}// namespace pf::ui::ig