//
// Created by xflajs00 on 10.05.2022.
//

#include "MemoryEditor.h"

namespace pf::ui::ig {

bool MemoryEditor::isReadOnly() const { return memoryEditor.ReadOnly; }

void MemoryEditor::setReadOnly(bool readOnly) { memoryEditor.ReadOnly = readOnly; }

bool MemoryEditor::isShowOptions() const { return memoryEditor.OptShowOptions; }

void MemoryEditor::setShowOptions(bool showOptions) { memoryEditor.OptShowOptions = showOptions; }

bool MemoryEditor::isDataPreview() const { return memoryEditor.OptShowDataPreview; }

void MemoryEditor::setDataPreview(bool dataPreview) { memoryEditor.OptShowDataPreview = dataPreview; }

unsigned int MemoryEditor::getColumnCount() const { return memoryEditor.Cols; }

void MemoryEditor::setColumnCount(unsigned int count) { memoryEditor.Cols = count; }

std::span<std::byte> MemoryEditor::getData() const { return data; }

void MemoryEditor::setData(std::span<std::byte> newData) { data = newData; }

void MemoryEditor::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  memoryEditor.DrawContents(data.data(), data.size());
}

}  // namespace pf::ui::ig