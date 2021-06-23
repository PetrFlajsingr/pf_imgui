//
// Created by petr on 10/31/20.
//

#include "InputText.h"
#include <imgui.h>
#include <toml++/toml_node_view.h>
#include <utility>

namespace pf::ui::ig {
InputText::InputText(const std::string &elementName, std::string label, const std::string &text,
                     TextInputType textInputType, std::size_t inputLengthLimit, TextTrigger trigger,
                     Flags<TextFilter> filters, Persistent persistent)
    : Text(elementName, text), Labellable(std::move(label)), ValueObservable(""), Savable(persistent),
      buffer(std::unique_ptr<char[]>(new char[inputLengthLimit + 1])), bufferLength(inputLengthLimit),
      inputType(textInputType) {
  setTextInner(text);
  for (auto flag : filters.getSetFlags()) { flags |= static_cast<uint32_t>(flag); }
  if (trigger == TextTrigger::Enter) { flags |= ImGuiInputTextFlags_EnterReturnsTrue; }
}

void InputText::renderImpl() {
  //auto colorStyle = setColorStack();
  //auto style = setStyleStack();
  auto valueChanged = false;
  if (inputType == TextInputType::SingleLine) {
    valueChanged = ImGui::InputText(getLabel().c_str(), buffer.get(), 256, flags);
  } else {
    valueChanged = ImGui::InputTextMultiline(getLabel().c_str(), buffer.get(), 256, ImVec2(0, 0), flags);
  }
  if (valueChanged && strcmp(buffer.get(), getText().c_str()) != 0) {
    setTextInner(buffer.get());
    setValueInner(getText());
    notifyValueChanged();
  }
  drag(getText());
  if (auto drop = dropAccept(); drop.has_value()) { setText(*drop); }
}
void InputText::clear() {
  setText("");
  buffer[0] = '\0';
}

void InputText::unserialize_impl(const toml::table &src) {
  setText(**src["text"].as_string());
  setValueAndNotifyIfChanged(getText());
  std::snprintf(buffer.get(), getText().size(), "%s", getText().c_str());
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
bool InputText::isPassword() const { return password; }
void InputText::setPassword(bool passwd) {
  password = passwd;
  if (password) {
    flags |= ImGuiInputTextFlags_Password;
  } else {
    flags &= ~ImGuiInputTextFlags_Password;
  }
}
void InputText::setTextInner(std::string txt) {
  if (txt.size() > bufferLength) { txt = txt.substr(bufferLength); }
  Text::setTextInner(txt);
  std::ranges::copy(txt, buffer.get());
  buffer[txt.size()] = '\0';
}

}// namespace pf::ui::ig