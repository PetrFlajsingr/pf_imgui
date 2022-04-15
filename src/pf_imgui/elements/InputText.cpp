//
// Created by petr on 10/31/20.
//

#include "InputText.h"
#include <algorithm>
#include <imgui.h>
#include <memory>
#include <utility>

namespace pf::ui::ig {

InputText::InputText(InputText::Config &&config)
    : ItemElement(std::string{config.name}), Labellable(std::string{config.label}), ValueObservable(""),
      Savable(config.persistent ? Persistent::Yes : Persistent::No), DragSource<std::string>(false),
      DropTarget<std::string>(false), text(std::move(config.value)),
      buffer(std::unique_ptr<char[]>(new char[config.maxInputLength + 1])), bufferLength(config.maxInputLength),
      inputType(config.inputType) {
  setTextInner(text);
  setValueInner(text);
  flags |= static_cast<ImGuiInputTextFlags>(*config.filters);
  if (config.eventTrigger == TextTrigger::Enter) { flags |= ImGuiInputTextFlags_EnterReturnsTrue; }
}

InputText::InputText(const std::string &elementName, std::string label, const std::string &value,
                     TextInputType textInputType, std::size_t inputLengthLimit, TextTrigger trigger,
                     const Flags<TextFilter> &filters, Persistent persistent)
    : ItemElement(elementName), Labellable(std::move(label)), ValueObservable(""),
      Savable(persistent), DragSource<std::string>(false), DropTarget<std::string>(false), text(value),
      buffer(std::unique_ptr<char[]>(new char[inputLengthLimit + 1])), bufferLength(inputLengthLimit),
      inputType(textInputType) {
  setTextInner(value);
  setValueInner(text);
  flags |= static_cast<ImGuiInputTextFlags>(*filters);
  if (trigger == TextTrigger::Enter) { flags |= ImGuiInputTextFlags_EnterReturnsTrue; }
}

void InputText::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  auto valueChanged = false;
  if (inputType == TextInputType::SingleLine) {
    valueChanged = ImGui::InputText(getLabel().c_str(), buffer.get(), 256, flags);
  } else {
    valueChanged = ImGui::InputTextMultiline(getLabel().c_str(), buffer.get(), 256, ImVec2(0, 0), flags);
  }
  if (valueChanged && strcmp(buffer.get(), text.c_str()) != 0) {
    text = buffer.get();
    setTextInner(buffer.get());
    setValueInner(text);
    notifyValueChanged();
  }
  drag(text);
  if (auto drop = dropAccept(); drop.has_value()) {
    if (text != *drop) {
      text = *drop;
      setTextInner(buffer.get());
      setValueInner(text);
      notifyValueChanged();
    }
  }
}

void InputText::clear() {
  text = "";
  buffer[0] = '\0';
}

toml::table InputText::toToml() const { return toml::table{{"text", text}}; }

void InputText::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("text"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<std::string>(); newVal.has_value()) {
      text = *newVal;
      setValueAndNotifyIfChanged(text);
    }
  }

  std::snprintf(buffer.get(), text.size(), "%s", text.c_str());
}

void InputText::setValue(const std::string_view &newValue) {
  if (text != newValue) {
    text = newValue;
    setTextInner(buffer.get());
    setValueInner(text);
    notifyValueChanged();
  }
}

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
  std::ranges::copy(txt, buffer.get());
  buffer[txt.size()] = '\0';
}

}  // namespace pf::ui::ig
