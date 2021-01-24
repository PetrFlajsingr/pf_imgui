//
// Created by petr on 11/2/20.
//

#include "Memo.h"
#include "Button.h"
#include "InputText.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>

namespace pf::ui::ig {

Memo::Memo(const std::string &elementName, const std::string &label, float textAHeight, bool buttonsEnabled,
           bool filterEnabled, const std::optional<std::size_t> &recordLimit)
    : Element(elementName), Labellable(label),
      textAreaLayout(elementName + "_memo_panel###", LayoutDirection::LeftToRight, ImVec2{0, textAHeight}),
      buttonsEnabled(buttonsEnabled), filterEnabled(filterEnabled), recordLimit(recordLimit) {}

void Memo::renderImpl() {
  if (rebuild) { rebuildPanel(); }
  removeRecordsAboveLimit();
  textArea->setText(getText());
  ImGui::Text("%s", getLabel().c_str());
  if (controlsLayout != nullptr) { controlsLayout->render(); }
  ImGui::Separator();
  textAreaLayout.render();
  ImGui::Separator();
}

const std::vector<std::string> &Memo::getRecords() const { return records; }

std::string Memo::getText() const {
  return records | ranges::views::filter(filterFnc) | ranges::views::join('\n') | ranges::to<std::string>();
}

void Memo::addRecord(std::string_view record) { records.emplace_back(std::string(record)); }

void Memo::removeRecord(std::size_t index) {
  if (index >= records.size()) { return; }
  records.erase(records.begin() + index);
}

void Memo::clearRecords() { records.clear(); }

void Memo::rebuildPanel() {
  if (buttonsEnabled || filterEnabled) {
    controlsLayout =
        std::make_unique<BoxLayout>(getName() + "button_filter_panel", LayoutDirection::TopToBottom, ImVec2{0, 20});
    if (buttonsEnabled) {
      controlsLayout->createChild<Button>(getName() + "clear_btn", "Clear").addClickListener([this] {
        clearRecords();
      });
      controlsLayout->createChild<Button>(getName() + "copy_btn", "Copy").addClickListener([this] {
        ImGui::SetClipboardText(getText().c_str());
      });
    }
    if (filterEnabled) {
      controlsLayout->createChild<InputText>(getName() + "filter_input", "Filter")
          .addValueListener([this](std::string_view str) {
            const auto filterStr = std::string(str);
            filterFnc = [filterStr](std::string_view recordStr) {
              return recordStr.find(filterStr) != std::string::npos;
            };
          });
    }
  }
  textArea = &textAreaLayout.createChild<Text>(getName() + "memo_text", "Memo");
  rebuild = false;
}
bool Memo::isButtonsEnabled() const { return buttonsEnabled; }

void Memo::setButtonsEnabled(bool enabled) {
  rebuild = true;
  buttonsEnabled = enabled;
}

bool Memo::isFilterEnabled() const { return filterEnabled; }

void Memo::setFilterEnabled(bool enabled) {
  rebuild = true;
  filterEnabled = enabled;
}

const std::optional<std::size_t> &Memo::getRecordLimit() const { return recordLimit; }

void Memo::setRecordLimit(std::size_t limit) { recordLimit = limit; }

void Memo::removeRecordsAboveLimit() {
  if (recordLimit.has_value() && *recordLimit < records.size()) {
    const auto cntToRemove = records.size() - *recordLimit;
    records.erase(records.begin(), records.begin() + cntToRemove);
  }
}

}// namespace pf::ui::ig