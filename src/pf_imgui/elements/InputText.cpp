//
// Created by petr on 10/31/20.
//

#include "InputText.h"
#include <imgui.h>
#include <toml++/toml_node_view.h>
#include <utility>

namespace pf::ui::ig {
InputText::InputText(const std::string &elementName, std::string label, const std::string &text,
                     TextInputType textInputType, TextTrigger trigger, Flags<TextFilter> filters, Persistent persistent)
    : Text(elementName, text), Labellable(std::move(label)), ValueObservable(""), Savable(persistent),
      inputType(textInputType) {
  setValueInner(text);
  for (auto flag : filters.getSetFlags()) { flags |= static_cast<uint>(flag); }
  if (trigger == TextTrigger::Enter) {
    flags |= ImGuiInputTextFlags_EnterReturnsTrue;
  }
}

void InputText::renderImpl() {
  auto valueChanged = false;
  if (inputType == TextInputType::SingleLine) {
    valueChanged = ImGui::InputText(getLabel().c_str(), buffer, 256, flags);
  } else {
    valueChanged = ImGui::InputTextMultiline(getLabel().c_str(), buffer, 256, ImVec2(0, 0), flags);
  }
  if (valueChanged && strcmp(buffer, getText().c_str()) != 0) {
    setText(buffer);
    setValueInner(getText());
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

void InputText::setValue(const std::string_view &newValue) { setText(std::string(newValue)); }

bool InputText::isReadOnly() const { return readOnly; }

void InputText::setReadOnly(bool isReadOnly) {
  readOnly = isReadOnly;
  if (readOnly) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  } else {
    flags &= ~ImGuiInputTextFlags_ReadOnly;
  }
}

}// namespace pf::ui::ig