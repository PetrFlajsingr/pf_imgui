//
// Created by petr on 11/2/20.
//

#include "Memo.h"
#include "Button.h"
#include "Checkbox.h"
#include "InputText.h"
#include <range/v3/range/conversion.hpp>
#include <pf_imgui/resources/ResourceFactory.h>

namespace pf::ui::ig {

Memo::Memo(const std::string &elementName, std::unique_ptr<Resource<std::string>> label, uint32_t textAHeight, bool buttonsEnabled,
           bool filterEnabled, const std::optional<std::size_t> &recordLimit)
    : Element(elementName), Labellable(std::move(label)),
      textAreaLayout(elementName + "_memo_panel###", LayoutDirection::TopToBottom, Size{Width::Auto(), textAHeight}),
      buttonsEnabled(buttonsEnabled), filterEnabled(filterEnabled), recordLimit(recordLimit) {
  textAreaLayout.setScrollable(true);
  rebuildPanel();
}

void Memo::renderImpl() {
  auto colorStyle = setColorStack();
  auto style = setStyleStack();
  if (rebuild) { rebuildPanel(); }
  removeRecordsAboveLimit();
  ImGui::Text("%s", getLabel().get().c_str());
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
  using namespace std::string_literals;
  if (buttonsEnabled || filterEnabled) {
    controlsLayout = std::make_unique<BoxLayout>(getName() + "button_filter_panel", LayoutDirection::LeftToRight,
                                                 Size{Width::Auto(), 20});
    if (buttonsEnabled) {
      controlsLayout->createChild<Button>(getName() + "clear_btn", makeConstResource("Clear"s)).addClickListener([this] {
        clearRecords();
      });
      controlsLayout->createChild<Button>(getName() + "copy_btn", makeConstResource("Copy"s)).addClickListener([this] {
        ImGui::SetClipboardText(std::string{getText()}.c_str());
      });
    }
    if (filterEnabled) {
      controlsLayout->createChild<InputText>(getName() + "filter_input", makeConstResource("Filter"s))
          .addValueListener([this](std::string_view str) {
            const auto filterStr = std::string(str);
            filterFnc = [filterStr](std::string_view recordStr) {
              return recordStr.find(filterStr) != std::string::npos;
            };
          });
    }
    controlsLayout->createChild<Checkbox>(getName() + "scroll_checkbox", makeConstResource("Scroll to bottom"s))
        .addValueListener([this](auto newVal) { scrollToBottom = newVal; });
  }
  textArea = &textAreaLayout.createChild<Text>(getName() + "memo_text", "Memo");
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
