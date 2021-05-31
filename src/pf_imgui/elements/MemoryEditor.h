/**
 * @file MemoryEditor.h
 * @brief Raw memory editor.
 * @author Petr Flajšingr
 * @date 23.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_MEMORYEDITOR_H
#define PF_IMGUI_ELEMENTS_MEMORYEDITOR_H

#include <imgui_memory_editor.h>
#include <pf_imgui/interface/ItemElement.h>
#include <ranges>

namespace pf::ui::ig {

/**
 * @brief Hex memory editor.
 *
 * Editor holds the provided range as a reference.
 *
 * @todo: more options
 */
template<std::ranges::contiguous_range R>
class MemoryEditor : public ItemElement {
 public:
  MemoryEditor(const std::string &elementName, R &data) : ItemElement(elementName), range(data) {}

  /**
   * Check if editor is read only.
   * @return
   */
  [[nodiscard]] bool isReadOnly() const { return memoryEditor.ReadOnly; }
  /**
   * Set editor to read only.
   * @param readOnly
   */
  void setReadOnly(bool readOnly) { memoryEditor.ReadOnly = readOnly; }
  /**
   * Check if options button is enabled.
   * @return
   */
  [[nodiscard]] bool isShowOptions() const { return memoryEditor.OptShowOptions; }
  /**
   * Enable/disable options button.
   * @param showOptions
   */
  void setShowOptions(bool showOptions) { memoryEditor.OptShowOptions = showOptions; }
  /**
   * Check if panel for previewing data in different formats is enabled.
   * @return
   */
  [[nodiscard]] bool isDataPreview() const { return memoryEditor.OptShowDataPreview; }
  /**
   * Enable/disable data preview panel
   * @param dataPreview
   */
  void setDataPreview(bool dataPreview) { memoryEditor.OptShowDataPreview = dataPreview; }
  /**
   * Get an amount of rendered values on a row.
   * @return an amount of rendered values on a row
   */
  [[nodiscard]] unsigned int getColumnCount() const { return memoryEditor.Cols; }
  /**
   * Set an amount of rendered values on a row.
   * @param count new column count
   */
  void setColumnCount(unsigned int count) { memoryEditor.Cols = count; }

  [[nodiscard]] R &getRange() const { return range; }
  void setRange(R &newRange) { range = newRange; }

 protected:
  void renderImpl() override {
    memoryEditor.DrawContents(std::ranges::data(range),
                              std::ranges::size(range) * sizeof(std::ranges::range_value_t<R>));
  }

 private:
  ImGui::MemoryEditor memoryEditor{};
  R &range;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_ELEMENTS_MEMORYEDITOR_H
