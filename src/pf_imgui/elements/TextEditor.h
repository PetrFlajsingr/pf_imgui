//
// Created by petr.flajsingr on 2/4/2022.
//

#ifndef PF_IMGUI_TEXTEDITOR_H
#define PF_IMGUI_TEXTEDITOR_H

#include <TextEditor.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>

namespace pf::ui::ig {
// TODO: styles/colors

class TextEditor : public Element, public Savable, public Resizable {
 public:
  TextEditor(const std::string &name, std::string_view value, Size s = Size::Auto(), Persistent persistent = Persistent::No);

  [[nodiscard]] ImGuiColorTextEdit::TextEditor &getEditor();
  [[nodiscard]] const ImGuiColorTextEdit::TextEditor &getEditor() const;

  [[nodiscard]] std::string getText() const;
  void setText(const std::string &text);

 protected:
  void renderImpl() override;

  void unserialize_impl(const toml::table &src) override;
  toml::table serialize_impl() override;

 private:
  ImGuiColorTextEdit::TextEditor editor;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_TEXTEDITOR_H
