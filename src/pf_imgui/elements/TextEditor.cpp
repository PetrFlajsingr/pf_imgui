//
// Created by petr.flajsingr on 2/4/2022.
//

#include "TextEditor.h"
#include <string>

namespace pf::ui::ig {

TextEditor::TextEditor(TextEditor::Config &&config)
    : Element(std::string{config.name}), Savable(config.persistent), Resizable(config.size) {
  editor.SetText(config.value);
}

TextEditor::TextEditor(const std::string &name, const std::string &value, Size s, Persistent persistent)
    : Element(name), Savable(persistent), Resizable(s) {
  editor.SetText(value);
}

ImGuiColorTextEdit::TextEditor &TextEditor::getEditor() { return editor; }

const ImGuiColorTextEdit::TextEditor &TextEditor::getEditor() const { return editor; }

std::string TextEditor::getText() const { return editor.GetText(); }

void TextEditor::setText(const std::string &text) { editor.SetText(text); }

void TextEditor::renderImpl() { editor.Render(getName().c_str(), getSize().asImVec()); }

void TextEditor::unserialize_impl(const toml::table &src) {
  if (auto newTextIter = src.find("text"); newTextIter != src.end()) {
    if (auto newText = newTextIter->second.as_string(); newText != nullptr) { editor.SetText(newText->get()); }
  }
}

toml::table TextEditor::serialize_impl() const { return toml::table{{"text", editor.GetText()}}; }

void TextEditor::setHighlighting(TextEditor::Highlight language) {
  switch (language) {
    case Highlight::GLSL:
      editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::GLSL());
      break;
    case Highlight::HLSL:
      editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::HLSL());
      break;
    case Highlight::AngelScript:
      editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::AngelScript());
      break;
    case Highlight::Lua: editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::Lua()); break;
    case Highlight::C: editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::C()); break;
    case Highlight::CPP:
      editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::CPlusPlus());
      break;
    case Highlight::SQL: editor.SetLanguageDefinition(ImGuiColorTextEdit::TextEditor::LanguageDefinition::SQL()); break;
  }
}

}  // namespace pf::ui::ig
