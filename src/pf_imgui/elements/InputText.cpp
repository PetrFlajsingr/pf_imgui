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
    : ItemElement(config.name.value), Savable(config.persistent ? Persistent::Yes : Persistent::No),
      DragSource<std::string>(false), DropTarget<std::string>(false), label(std::string{config.label.value}),
      text(std::move(config.value)), buffer(std::make_unique<char[]>(config.maxInputLength + 1)),
      bufferLength(config.maxInputLength), inputType(config.inputType) {
  setTextInner(*text);
  flags |= static_cast<ImGuiInputTextFlags>(*config.filters);
  if (config.eventTrigger == TextTrigger::Enter) { flags |= ImGuiInputTextFlags_EnterReturnsTrue; }
  text.addListener([this](const auto &newText) { setTextInner(newText); });
}

InputText::InputText(std::string_view elementName, std::string_view labelText, std::string_view initialValue,
                     TextInputType textInputType, std::size_t inputLengthLimit, TextTrigger trigger,
                     const Flags<TextFilter> &filters, Persistent persistent)
    : ItemElement(elementName), Savable(persistent), DragSource<std::string>(false), DropTarget<std::string>(false),
      label(std::string{labelText}), text(std::string{initialValue}), buffer(std::make_unique<char[]>(inputLengthLimit + 1)),
      bufferLength(inputLengthLimit), inputType(textInputType) {
  setTextInner(*text);
  flags |= static_cast<ImGuiInputTextFlags>(*filters);
  if (trigger == TextTrigger::Enter) { flags |= ImGuiInputTextFlags_EnterReturnsTrue; }
  text.addListener([this](const auto &newText) { setTextInner(newText); });
}

void InputText::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto valueChanged = false;
  if (inputType == TextInputType::SingleLine) {
    valueChanged = ImGui::InputText(label->get().c_str(), buffer.get(), 256, flags);
  } else {
    valueChanged = ImGui::InputTextMultiline(label->get().c_str(), buffer.get(), 256, ImVec2(0, 0), flags);
  }
  if (valueChanged && strcmp(buffer.get(), text->c_str()) != 0) {
    setTextInner(buffer.get());
    *text.modify() = buffer.get();
  }
  drag(*text);
  if (auto drop = dropAccept(); drop.has_value()) {
    if (*text != *drop) {
      setTextInner(buffer.get());
      *text.modify() = *drop;
    }
  }
}

void InputText::clear() {
  buffer[0] = '\0';
  text.modify()->clear();
}

toml::table InputText::toToml() const { return toml::table{{"text", *text}}; }

void InputText::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("text"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.value<std::string>(); newVal.has_value()) { *text.modify() = *newVal; }
  }

  std::snprintf(buffer.get(), text->size(), "%s", text->c_str());
}

void InputText::setValue(const std::string &newValue) { *text.modify() = newValue; }

const std::string &InputText::getValue() const { return *text; }

Subscription InputText::addValueListenerImpl(std::function<void(const std::string &)> listener) {
  return text.addListener(std::move(listener));
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

TextInputType InputText::getInputType() const { return inputType; }

void InputText::setInputType(TextInputType newInputType) { inputType = newInputType; }

TextTrigger InputText::getInputTrigger() const {
  if (flags & ImGuiInputTextFlags_EnterReturnsTrue) {
    return TextTrigger::Enter;
  } else {
    return TextTrigger::Character;
  }
}

void InputText::setInputTrigger(TextTrigger trigger) {
  if (trigger == TextTrigger::Enter) {
    flags |= ImGuiInputTextFlags_EnterReturnsTrue;
  } else {
    flags &= ~ImGuiInputTextFlags_EnterReturnsTrue;
  }
}

Flags<TextFilter> InputText::getFilters() const {
  const Flags<TextFilter> allFlags{magic_enum::enum_values<TextFilter>()};
  const auto setFilterFlags = flags & static_cast<ImGuiInputTextFlags>(*allFlags);
  return Flags<TextFilter>{static_cast<TextFilter>(setFilterFlags)};
}

void InputText::setFilters(Flags<TextFilter> filters) {  //-V813
  const Flags<TextFilter> allFlags{magic_enum::enum_values<TextFilter>()};
  flags &= ~static_cast<ImGuiInputTextFlags>(*allFlags);
  flags |= static_cast<ImGuiInputTextFlags>(*filters);
}

}  // namespace pf::ui::ig
