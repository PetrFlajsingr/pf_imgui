//
// Created by petr on 11/2/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_MEMO_H
#define PF_IMGUI_IMGUI_ELEMENTS_MEMO_H

#include "../fwd.h"
#include "Panel.h"
#include "interface/LabeledElement.h"
#include <memory>
#include <optional>
#include <pf_imgui/_export.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Memo : public LabeledElement {
 public:
  Memo(const std::string &elementName, const std::string &caption, float textAHeight = 0,
       bool buttonsEnabled = true, bool filterEnabled = true,
       const std::optional<std::size_t> &recordLimit = std::nullopt);

  [[nodiscard]] const std::vector<std::string> &getRecords() const;
  [[nodiscard]] std::string getText() const;

  void addRecord(std::string_view record);
  void removeRecord(std::size_t index);
  void clearRecords();

  [[nodiscard]] bool isButtonsEnabled() const;
  void setButtonsEnabled(bool buttonsEnabled);
  [[nodiscard]] bool isFilterEnabled() const;
  void setFilterEnabled(bool filterEnabled);
  [[nodiscard]] const std::optional<std::size_t> &getRecordLimit() const;
  void setRecordLimit(std::size_t limit);

 protected:
  void renderImpl() override;

 private:
  void rebuildPanel();
  void removeRecordsAboveLimit();

  std::unique_ptr<Panel> controlsPanel = nullptr;
  Panel textAreaPanel;
  bool buttonsEnabled;
  bool filterEnabled;
  bool rebuild = true;
  std::function<bool(std::string_view)> filterFnc = [](auto) { return true; };
  std::optional<std::size_t> recordLimit;
  std::vector<std::string> records;
  Text *textArea;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_MEMO_H
