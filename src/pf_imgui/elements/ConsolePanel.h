//
// Created by Petr on 6/23/2022.
//

#ifndef PF_IMGUI_ELEMENTS_CONSOLEPANEL_H
#define PF_IMGUI_ELEMENTS_CONSOLEPANEL_H

#include <pf_imgui/details/CustomIconButtonImpls.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>

namespace pf::ui::ig {

class ConsolePanel : public ElementWithID, public Resizable, public Savable {
  enum class RecordType { Input, Output };

 public:
  struct Record {
    std::string message;
    std::optional<Color> color;
  };

  ConsolePanel(const std::string &name, Size size, Persistent persistent = Persistent::No);

  void setInputHandler(std::invocable<std::string_view> auto &&fnc)
    requires(std::same_as<std::invoke_result_t<decltype(fnc), std::string_view>, std::optional<Record>>)
  {
    inputHandler = std::forward<decltype(fnc)>(fnc);
  }

  [[nodiscard]] std::string getOutput() const;

  void addCompletionString(std::string str) { completionStrings.emplace_back(std::move(str)); }

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  void handleSubmit();

  static int CompletionCallback(ImGuiInputTextCallbackData *data);
  std::vector<std::pair<RecordType, Record>> records;
  std::function<std::optional<Record>(std::string_view)> inputHandler = [](auto) -> std::optional<Record> {
    return {};
  };

  WrapTextToggle wrapTextToggle;
  ScrollToEndToggle scrollToEndToggle;
  CopyToClipboardButton copyToClipboardButton;
  TrashcanButton clearButton;

  constexpr static std::size_t BUFFER_SIZE = 512;
  char inputBuffer[BUFFER_SIZE] = "\0";

  std::vector<std::string> history;
  std::optional<std::size_t> historyIndex = std::nullopt;

  std::vector<std::string> completionStrings;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_CONSOLEPANEL_H
