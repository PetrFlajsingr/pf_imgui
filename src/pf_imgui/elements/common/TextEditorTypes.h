//
// Created by xflajs00 on 20.04.2022.
//

#ifndef PF_IMGUI_ELEMENTS_COMMON_TEXTEDITORTYPES_H
#define PF_IMGUI_ELEMENTS_COMMON_TEXTEDITORTYPES_H

#include <ImGuiColorTextEditor.h>

namespace pf::ui::ig {

enum class TextEditorHighlight { GLSL, HLSL, AngelScript, Lua, C, CPP, SQL };

// TODO: implement properly in the text editor sometime
// for now only works for line
struct Breakpoint {  //-V730
  std::uint32_t line;
  bool enabled = true;
  std::string text{};
};

struct TextEditorMarker {
  std::uint32_t line;
  std::string text;
};

struct TextCursorPosition {
  std::uint32_t line;
  std::uint32_t column;
};

enum class TextEditorColor {
  Default = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Default),
  Keyword = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Keyword),
  Number = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Number),
  String = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::String),
  CharLiteral = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::CharLiteral),
  Punctuation = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Punctuation),
  Preprocessor = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Preprocessor),
  Identifier = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Identifier),
  KnownIdentifier = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::KnownIdentifier),
  PreprocIdentifier = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::PreprocIdentifier),
  Comment = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Comment),
  MultiLineComment = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::MultiLineComment),
  Background = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Background),
  Cursor = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Cursor),
  Selection = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Selection),
  ErrorMarker = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::ErrorMarker),
  WarningMarker = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::WarningMarker),
  Breakpoint = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::Breakpoint),
  LineNumber = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::LineNumber),
  CurrentLineFill = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::CurrentLineFill),
  CurrentLineFillInactive = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::CurrentLineFillInactive),
  CurrentLineEdge = static_cast<int>(ImGuiColorTextEdit::TextEditor::PaletteIndex::CurrentLineEdge),
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_COMMON_TEXTEDITORTYPES_H
