//
// Created by Petr on 6/23/2022.
//

#include "ConsolePanel.h"
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

namespace pf::ui::ig {

ConsolePanel::ConsolePanel(const std::string &name, Size size, Persistent persistent)
    : ElementWithID(name), Resizable(size), Savable(persistent), wrapTextToggle("wrapText"),
      scrollToEndToggle("scrollToEnd"), copyToClipboardButton("copyToClipboard"), clearButton("clear") {

  clearButton.addClickListener([this] { records.clear(); });
  copyToClipboardButton.addClickListener([this] { ImGui::SetClipboardText(getOutput().c_str()); });
  clearButton.setTooltip("Clear output");
  copyToClipboardButton.setTooltip("Copy output to clipboard");
  wrapTextToggle.setTooltip("Wrap text");
  scrollToEndToggle.setTooltip("Autoscroll down");
}

std::string ConsolePanel::getOutput() const {
  return records | ranges::views::transform(&std::pair<RecordType, Record>::second)
      | ranges::views::transform(&Record::message) | ranges::views::join('\n') | ranges::to<std::string>();
}

void ConsolePanel::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();

  ImGui::BeginHorizontal("layout", static_cast<ImVec2>(getSize()), 0);
  {
    ImGui::BeginChild("controls_area", ImVec2{25, 0});
    {
      ImGui::BeginVertical("controls", ImVec2{0, 0}, 0);
      {
        wrapTextToggle.render();
        ImGui::Spring(0);
        scrollToEndToggle.render();
        ImGui::Spring(0);
        copyToClipboardButton.render();
        ImGui::Spring(0);
        clearButton.render();
      }
      ImGui::EndVertical();
    }
    ImGui::EndChild();
    ImGui::Spring(0.f);
    ImGui::BeginVertical("output", ImVec2{0, 0}, 0);
    {
      {
        RAII end{ImGui::EndChild};
        const auto wrapEnabled = wrapTextToggle.getValue();
        if (ImGui::BeginChild("out_w", ImVec2{0, -25}, true,
                              wrapEnabled ? ImGuiWindowFlags{} : ImGuiWindowFlags_HorizontalScrollbar)) {
          std::ranges::for_each(records, [&](const auto &record) {
            std::string prefix{};
            if (record.first == RecordType::Input) { prefix = "#"; }
            if (wrapEnabled) {
              ImGui::TextWrapped((prefix + record.second.message).c_str());
            } else {
              ImGui::Text((prefix + record.second.message).c_str());
            }
          });
          if (scrollToEndToggle.getValue()) { ImGui::SetScrollHereY(1.0f); }
        }
      }

      ImGui::BeginHorizontal("user_in", ImVec2{0, 0}, 0);
      {
        ImGui::SetNextItemWidth(-60.f);
        // TODO: completion using callback
        if (ImGui::InputText("##input", inputBuffer, BUFFER_SIZE,
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory
                                 | ImGuiInputTextFlags_CallbackCompletion,
                             CompletionCallback, this)) {
          handleSubmit();
          ImGui::SetKeyboardFocusHere(-1);
        }

        if (ImGui::Button("Submit")) { handleSubmit(); }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }
  ImGui::EndHorizontal();
}

void ConsolePanel::handleSubmit() {
  if (inputBuffer[0] != '\0') {
    records.emplace_back(RecordType::Input, Record{inputBuffer, std::nullopt});
    if (auto response = inputHandler(std::string_view{inputBuffer}); response.has_value()) {
      records.emplace_back(RecordType::Output, std::move(*response));
    }
    history.emplace_back(inputBuffer);
    inputBuffer[0] = '\0';
    historyIndex = std::nullopt;
  }
}

int ConsolePanel::CompletionCallback(ImGuiInputTextCallbackData *data) {
  auto &self = *reinterpret_cast<ConsolePanel *>(data->UserData);
  auto historyChanged = false;
  switch (data->EventKey) {
    case ImGuiKey_UpArrow:
      if (!self.history.empty()) {
        if (!self.historyIndex.has_value()) {
          self.historyIndex = self.history.size() - 1;
          historyChanged = true;
        } else if (*self.historyIndex > 0) {
          --*self.historyIndex;
          historyChanged = true;
        }
      }
      break;
    case ImGuiKey_DownArrow:
      if (self.historyIndex.has_value() && *self.historyIndex < self.history.size() - 1) {
        ++*self.historyIndex;
        historyChanged = true;
      }
      break;
    case ImGuiKey_Tab:
      if (const auto iter = std::ranges::find_if(self.completionStrings,
                                                 [data](const auto &str) { return str.starts_with(data->Buf); });
          iter != self.completionStrings.end()) {
        strcpy_s(data->Buf, BUFFER_SIZE, iter->c_str());
        data->BufDirty = true;
        data->BufTextLen = static_cast<int>(iter->size());
      }
      break;
  }
  if (historyChanged) {
    strcpy_s(data->Buf, BUFFER_SIZE, self.history[*self.historyIndex].c_str());
    data->BufDirty = true;
    data->BufTextLen = static_cast<int>(self.history[*self.historyIndex].size());
    data->CursorPos = data->BufTextLen;
  }
  return 0;
}

}  // namespace pf::ui::ig