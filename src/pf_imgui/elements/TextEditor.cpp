//
// Created by petr.flajsingr on 2/4/2022.
//

#include "TextEditor.h"

namespace pf::ui::ig {

TextEditor::TextEditor(const std::string &name, std::string_view value, Size s, Persistent persistent)
    : Element(name), Savable(persistent), Resizable(s) {
  editor.SetText(std::string{value});
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

toml::table TextEditor::serialize_impl() { return toml::table{{{"text", editor.GetText()}}}; }

}// namespace pf::ui::ig