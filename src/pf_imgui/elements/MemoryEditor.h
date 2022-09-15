/**
 * @file MemoryEditor.h
 * @brief Raw memory editor.
 * @author Petr Flajšingr
 * @date 23.5.21
 */

#ifndef PF_IMGUI_ELEMENTS_MEMORYEDITOR_H
#define PF_IMGUI_ELEMENTS_MEMORYEDITOR_H

#include <imgui_memory_editor.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/style/ColorPalette.h>
#include <pf_imgui/style/StyleOptions.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Hex memory editor.
 *
 * Editor holds the provided range as a reference.
 *
 * @todo: more options
 */
class PF_IMGUI_EXPORT MemoryEditor : public ItemElement {
 public:
  template<typename T>
  MemoryEditor(std::string_view elementName, std::span<T> newData);

  /**
   * Check if editor is read only.
   * @return
   */
  [[nodiscard]] bool isReadOnly() const;
  /**
   * Set editor to read only.
   * @param readOnly
   */
  void setReadOnly(bool readOnly);
  /**
   * Check if options button is enabled.
   * @return
   */
  [[nodiscard]] bool isShowOptions() const;
  /**
   * Enable/disable options button.
   * @param showOptions
   */
  void setShowOptions(bool showOptions);
  /**
   * Check if panel for previewing data in different formats is enabled.
   * @return
   */
  [[nodiscard]] bool isDataPreview() const;
  /**
   * Enable/disable data preview panel
   * @param dataPreview
   */
  void setDataPreview(bool dataPreview);
  /**
   * Get an amount of rendered values on a row.
   * @return an amount of rendered values on a row
   */
  [[nodiscard]] unsigned int getColumnCount() const;
  /**
   * Set an amount of rendered values on a row.
   * @param count new column count
   */
  void setColumnCount(unsigned int count);

  [[nodiscard]] std::span<std::byte> getData() const;
  void setData(std::span<std::byte> newData);

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  ImGui::MemoryEditor memoryEditor{};
  std::span<std::byte> data;
};

template<typename T>
MemoryEditor::MemoryEditor(std::string_view elementName, std::span<T> newData)
    : ItemElement(elementName),
      data(std::span{reinterpret_cast<const std::byte *>(newData.data()), newData.size() * sizeof(T)}) {}

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_MEMORYEDITOR_H
