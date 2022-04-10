//
// Created by petr on 11/2/20.
//

#include "Memo.h"
#include "Button.h"
#include "Checkbox.h"
#include "InputText.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

Memo::Memo(Memo::Config &&config)
    : Element(std::string{config.name}), Labellable(std::string{config.label}),
      textAreaLayout("memo_panel", Size{Width::Auto(), config.textAreaHeight}), buttonsEnabled(config.buttonsEnabled),
      filterEnabled(config.filterEnabled), recordLimit(config.maxRecordCount) {
  textAreaLayout.setScrollable(true);
  rebuildPanel();
}

Memo::Memo(const std::string &elementName, const std::string &label, uint32_t textAHeight, bool buttonsEnabled,
           bool filterEnabled, const std::optional<std::size_t> &recordLimit)
    : Element(elementName), Labellable(label), textAreaLayout("memo_panel", Size{Width::Auto(), textAHeight}),
      buttonsEnabled(buttonsEnabled), filterEnabled(filterEnabled), recordLimit(recordLimit) {
  textAreaLayout.setScrollable(true);
  rebuildPanel();
}

void Memo::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  if (rebuild) { rebuildPanel(); }
  removeRecordsAboveLimit();
  ImGui::Text("%s", getLabel().c_str());
  if (controlsLayout != nullptr) { controlsLayout->render(); }
  ImGui::Separator();
  if (scrollToBottom) { textAreaLayout.setScrollPosition(Layout::ScrollPosition::Bottom); }
  textAreaLayout.render();
  ImGui::Separator();
}

const std::vector<std::string> &Memo::getRecords() const { return records; }

std::string_view Memo::getText() const { return textArea->getText(); }

void Memo::addRecord(std::string_view record) {
  records.emplace_back(std::string(record));
  rebuildText();
}

void Memo::removeRecord(std::size_t index) {
#ifndef _MSC_VER  // TODO: MSVC internal compiler error
  if (index >= records.size()) { throw InvalidArgumentException{"Index out of bounds"}; }
#endif
  records.erase(records.begin() + static_cast<long long>(index));
  rebuildText();
}

void Memo::clearRecords() {
  records.clear();
  rebuildText();
}

void Memo::rebuildPanel() {
  if (buttonsEnabled || filterEnabled) {
    controlsLayout = std::make_unique<HorizontalLayout>("button_filter_panel", Size{Width::Auto(), 20});
    if (buttonsEnabled) {
      controlsLayout->createChild(Button::Config{"clear_btn", "Clear"}).addClickListener([this] { clearRecords(); });
      controlsLayout->createChild(Button::Config{"copy_btn", "Copy"}).addClickListener([this] {
        ImGui::SetClipboardText(std::string{getText()}.c_str());
      });
    }
    if (filterEnabled) {
      controlsLayout->createChild<InputText>("filter_input", "Filter").addValueListener([this](std::string_view str) {
        const auto filterStr = std::string(str);
        filterFnc = [filterStr](std::string_view recordStr) { return recordStr.find(filterStr) != std::string::npos; };
      });
    }
    controlsLayout->createChild(Checkbox::Config{"scroll_checkbox", "Scroll to bottom"})
        .addValueListener([this](auto newVal) { scrollToBottom = newVal; });
  }
  textArea = &textAreaLayout.createChild<Text>("memo_text", "Memo");
  rebuildText();
  rebuild = false;
}
bool Memo::isButtonsEnabled() const { return buttonsEnabled; }

void Memo::setButtonsEnabled(bool btnEnabled) {
  rebuild = true;
  buttonsEnabled = btnEnabled;
}

bool Memo::isFilterEnabled() const { return filterEnabled; }

void Memo::setFilterEnabled(bool filtEnabled) {
  rebuild = true;
  filterEnabled = filtEnabled;
}

const std::optional<std::size_t> &Memo::getRecordLimit() const { return recordLimit; }

void Memo::setRecordLimit(std::size_t limit) { recordLimit = limit; }

void Memo::removeRecordsAboveLimit() {
  if (recordLimit.has_value() && *recordLimit < records.size()) {
    const auto cntToRemove = records.size() - *recordLimit;
    records.erase(records.begin(), records.begin() + static_cast<long long>(cntToRemove));
    rebuildText();
  }
}
std::size_t Memo::size() const { return records.size(); }

void Memo::cancelRecordLimit() { recordLimit = std::nullopt; }

void Memo::rebuildText() { textArea->setText(getTextView() | ranges::to<std::string>()); }

std::vector<Renderable *> Memo::getRenderables() {
  // TODO: implement this
  return {};
}

}  // namespace pf::ui::ig
