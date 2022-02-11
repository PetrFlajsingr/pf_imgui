//
// Created by petr.flajsingr on 2/4/2022.
//

#ifndef PF_IMGUI_TEXTEDITOR_H
#define PF_IMGUI_TEXTEDITOR_H

#include <ImGuiColorTextEditor.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <string>

namespace pf::ui::ig {
// TODO: styles/colors

class PF_IMGUI_EXPORT TextEditor : public Element, public Savable, public Resizable {
 public:
  enum class Highlight { GLSL, HLSL, AngelScript, Lua, C, CPP, SQL };

  /**
   * @brief Struct for construction of TextEditor.
   */
  struct Config {
    using Parent = TextEditor;
    std::string_view name;                  /*!< Unique name of the element */
    std::string value{};                    /*!< Initial text inside the editor */
    Size size = Size::Auto();               /*!< Size of the element */
    Persistent persistent = Persistent::No; /*!< Allow state saving to disk */
  };
  /**
   * Construct TextEditor
   * @param config construction args @see TextEditor::Config
   */
  explicit TextEditor(Config &&config);

  TextEditor(const std::string &name, const std::string &value, Size s = Size::Auto(),
             Persistent persistent = Persistent::No);

  [[nodiscard]] ImGuiColorTextEdit::TextEditor &getEditor();
  [[nodiscard]] const ImGuiColorTextEdit::TextEditor &getEditor() const;

  [[nodiscard]] std::string getText() const;
  void setText(const std::string &text);

  void setHighlighting(Highlight language);

 protected:
  void renderImpl() override;

  void unserialize_impl(const toml::table &src) override;
  [[nodiscard]] toml::table serialize_impl() const override;

 private:
  ImGuiColorTextEdit::TextEditor editor;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_TEXTEDITOR_H
