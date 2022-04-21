//
// Created by petr.flajsingr on 2/4/2022.
//

#ifndef PF_IMGUI_TEXTEDITOR_H
#define PF_IMGUI_TEXTEDITOR_H

#include "common/TextEditorTypes.h"
#include <ImGuiColorTextEditor.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/Position.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <range/v3/view/transform.hpp>
#include <string>

namespace pf::ui::ig {
// TODO: styles/colors

class PF_IMGUI_EXPORT TextEditor : public Element, public Savable, public Resizable {
 public:
  class Cursor {
    friend class TextEditor;

   public:
    Cursor &insertTextAtCursorPos(const std::string &text);
    Cursor &moveUp(std::uint32_t lineCnt = 1, bool addToSelection = false);
    Cursor &moveDown(std::uint32_t lineCnt = 1, bool addToSelection = false);
    Cursor &moveLeft(std::uint32_t lineCnt = 1, bool addToSelection = false);
    Cursor &moveRight(std::uint32_t lineCnt = 1, bool addToSelection = false);
    Cursor &moveTop(bool addToSelection = false);
    Cursor &moveBottom(bool addToSelection = false);
    Cursor &moveHome(bool addToSelection = false);
    Cursor &moveEnd(bool addToSelection = false);
    Cursor &setSelectionStart(TextCursorPosition position);
    Cursor &setSelectionEnd(TextCursorPosition position);
    Cursor &selectAll();
    [[nodiscard]] bool hasSelection() const;
    Cursor &selectCurrentWord();
    Cursor &cutToClipboard();
    Cursor &pasteFromClipboard();
    Cursor &deleteSelection();
    Cursor &setPosition(TextCursorPosition position);
    [[nodiscard]] TextCursorPosition getPosition() const;
    [[nodiscard]] std::string getSelectedText() const;
    Subscription addPositionListener(std::invocable<TextCursorPosition> auto &&listener) {
      return owner.observableCursorPosition.addListener(std::forward<decltype(listener)>(listener));
    }

   private:
    explicit Cursor(TextEditor &parent);
    TextEditor &owner;
  };
  /**
   * @brief Struct for construction of TextEditor.
   */
  struct Config {
    using Parent = TextEditor;
    std::string_view name;    /*!< Unique name of the element */
    std::string value{};      /*!< Initial text inside the editor */
    Size size = Size::Auto(); /*!< Size of the element */
    bool persistent = false;  /*!< Allow state saving to disk */
  };
  /**
   * Construct TextEditor
   * @param config construction args @see TextEditor::Config
   */
  explicit TextEditor(Config &&config);

  TextEditor(const std::string &name, const std::string &value, Size s = Size::Auto(),
             Persistent persistent = Persistent::No);

  // TODO: maybe change this into a builder
  [[nodiscard]] Color getColor(TextEditorColor colorType) const;
  void setColor(TextEditorColor colorType, Color color);

  [[nodiscard]] auto getBreakpoints() const {
    return editor.GetBreakpoints() | ranges::views::transform([](int bpLine) {
             return Breakpoint{static_cast<uint32_t>(bpLine), true, {}};
           });
  }
  void clearBreakpoints();
  void addBreakpoint(const Breakpoint &breakpoint);
  void removeBreakpoint(std::uint32_t line);
  // TODO: warning markers, info markers

  [[nodiscard]] auto getErrorMarkers() const {
    return editor.GetErrorMarkers() | ranges::views::transform([](const auto &errMarker) {
             return TextEditorMarker{static_cast<uint32_t>(errMarker.first), errMarker.second};
           });
  }
  void clearErrorMarkers();
  void addErrorMarker(const TextEditorMarker &marker);
  void removeErrorMarker(std::uint32_t line);

  [[nodiscard]] auto getWarningMarkers() const {
    return editor.GetWarningMarkers() | ranges::views::transform([](const auto &errMarker) {
             return TextEditorMarker{static_cast<uint32_t>(errMarker.first), errMarker.second};
           });
  }
  void clearWarningMarkers();
  void addWarningMarker(const TextEditorMarker &marker);
  void removeWarningMarker(std::uint32_t line);

  [[nodiscard]] std::uint32_t getTabSize() const;
  void setTabSize(std::uint32_t tabSize);

  [[nodiscard]] Cursor getCursor();

  [[nodiscard]] ImGuiColorTextEdit::TextEditor &getEditor();
  [[nodiscard]] const ImGuiColorTextEdit::TextEditor &getEditor() const;

  [[nodiscard]] std::string getText() const;
  void setText(const std::string &text);
  Subscription addTextListener(std::invocable<std::string_view> auto &&listener) {
    return observableText.addListener(std::forward<decltype(listener)>(listener));
  }

  [[nodiscard]] bool canUndo() const;
  [[nodiscard]] bool canRedo() const;
  bool undo();
  bool redo();

  [[nodiscard]] bool isReadOnly() const;
  void setReadOnly(bool readOnly);

  // todo: custom highlighting&adding types, macros etc
  void setHighlighting(TextEditorHighlight language);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  ImGuiColorTextEdit::TextEditor editor;
  Observable_impl<std::string_view> observableText;
  Observable_impl<TextCursorPosition> observableCursorPosition;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_TEXTEDITOR_H
