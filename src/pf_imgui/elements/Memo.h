/**
 * @file Memo.h
 * @brief Memo element.
 * @author Petr Flajšingr
 * @date 2.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_MEMO_H
#define PF_IMGUI_ELEMENTS_MEMO_H

#include <memory>
#include <optional>
#include <pf_imgui/_export.h>
#include <pf_imgui/fwd.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/RenderablesContainer.h>
#include <pf_imgui/layouts/BoxLayout.h>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Multiline text overview with basic controls.
 *
 * User can add text by rows. Rows can be filtered using an Input. Copy and clear buttons can be enabled.
 *
 * @todo: rewrite this, there are some issues with button panel placement
 */
class PF_IMGUI_EXPORT Memo : public Element, public Labellable, public RenderablesContainer {
 public:
  struct Config {
    using Parent = Memo;
    std::string_view name;
    std::string_view label;
    Height textAreaHeight = Height::Auto();
    bool buttonsEnabled = true;
    bool filterEnabled = true;
    std::optional<std::size_t> maxRecordCount = std::nullopt;
  };
  explicit Memo(Config &&config);
  /**
   * Construct Memo.
   * @param elementName ID of the memo
   * @param label text rendered above the memo
   * @param textAHeight height of the text area
   * @param buttonsEnabled enable button for copy and clear
   * @param filterEnabled enable input for filter
   * @param recordLimit limit rows - if size is bigger than limit then the old rows are erased
   */
  Memo(const std::string &elementName, const std::string &label, uint32_t textAHeight = 0, bool buttonsEnabled = true,
       bool filterEnabled = true, const std::optional<std::size_t> &recordLimit = std::nullopt);

  /**
   * Get all records which are currently rendered.
   * @return all records within the memo
   */
  [[nodiscard]] const std::vector<std::string> &getRecords() const;
  /**
   * Get count of rows in the memo.
   * @return count of rows in the memo
   */
  [[nodiscard]] std::size_t size() const;
  /**
   * Get all records concatenated using '\n'.
   * @return all records as one string
   */
  [[nodiscard]] std::string_view getText() const;

  /**
   * Add new row to the memo.
   * @param record new row
   */
  void addRecord(std::string_view record);
  /**
   * Remove text at index.
   * @param index index to remove text at
   * @throws InvalidArgumentException when index is out of bounds
   */
  void removeRecord(std::size_t index);
  /**
   * Remove all records.
   */
  void clearRecords();

  /**
   * Check if copy and clear buttons are enabled.
   * @return true if copy and clear buttons are enabled, false otherwise
   */
  [[nodiscard]] bool isButtonsEnabled() const;
  /**
   * Set copy and clear buttons being enabled.
   * @param btnEnabled
   */
  void setButtonsEnabled(bool btnEnabled);
  /**
   * Check if filter is enabled.
   * @return true if filter is enabled, false otherwise
   */
  [[nodiscard]] bool isFilterEnabled() const;
  /**
   * Set filter being enabled.
   * @param filtEnabled
   */
  void setFilterEnabled(bool filtEnabled);
  /**
   * Get current record count limit.
   * @return current record count limit
   */
  [[nodiscard]] const std::optional<std::size_t> &getRecordLimit() const;
  /**
   * Set new record limit.
   * @param limit new limit
   */
  void setRecordLimit(std::size_t limit);
  /**
   * Cancel record limit.
   */
  void cancelRecordLimit();

  std::vector<Renderable *> getRenderables() override;

 protected:
  void renderImpl() override;

 private:
  void rebuildPanel();
  void removeRecordsAboveLimit();

  void rebuildText();
  auto getTextView() { return records | ranges::views::filter(filterFnc) | ranges::views::join('\n'); }

  std::unique_ptr<BoxLayout> controlsLayout = nullptr;
  BoxLayout textAreaLayout;
  bool buttonsEnabled;
  bool filterEnabled;
  bool rebuild = false;
  bool scrollToBottom = false;
  std::function<bool(std::string_view)> filterFnc = [](auto) { return true; };
  std::optional<std::size_t> recordLimit;
  std::vector<std::string> records;
  Text *textArea = nullptr;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_MEMO_H
