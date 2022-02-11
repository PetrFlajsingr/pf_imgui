/**
 * @file CustomCombobox.h
 * @brief Customizable combobox.
 * @author Petr Flajšingr
 * @date 4.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H

#include <imgui.h>
#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomItemBox.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

enum class ComboBoxCount { Items4 = 1 << 1, Items8 = 1 << 2, Items20 = 1 << 3, ItemsAll = 1 << 4 };

// TODO:
// ImGuiComboFlags_PopupAlignLeft
// ImGuiComboFlags_NoArrowButton
// ImGuiComboFlags_NoPreview

/**
 * @brief Combobox where rows can be whatever is provided by the user.
 *
 * Rows are created using a factory from stored value.
 * @tparam T type stored in each row
 * @tparam R type stored in each row
 *
 * @todo: Config
 */
template<typename T, std::derived_from<Renderable> R>
class PF_IMGUI_EXPORT CustomCombobox : public CustomItemBox<T, R>, public Labellable {
 public:
  /**
   * Construct CustomCombobox.
   * @param elementName ID of the element
   * @param label text rendered next to the element
   * @param rowFactory factory for renderable rows
   * @param prevValue preview value
   * @param showItemCount amount of items shown when open
   */
  CustomCombobox(const std::string &elementName, const std::string &label, CustomItemBoxFactory<T, R> auto &&rowFactory,
                 std::string prevValue = "", ComboBoxCount showItemCount = ComboBoxCount::Items8)
      : CustomItemBox<T, R>(elementName, std::forward<decltype(rowFactory)>(rowFactory)), Labellable(label),
        flags(static_cast<ImGuiComboFlags_>(showItemCount)), previewValue(std::move(prevValue)) {
    if (previewValue.empty()) { flags |= ImGuiComboFlags_::ImGuiComboFlags_NoPreview; }
  }

  void setPreviewValue(std::string value) {
    previewValue = std::move(value);
    if (previewValue.empty()) { flags |= ImGuiComboFlags_::ImGuiComboFlags_NoPreview; }
  }
  [[nodiscard]] const std::string &getPreviewValue() const { return previewValue; }

  /**
   * Get count of items shown when the element is unrolled.
   * @return count of items
   */
  [[nodiscard]] ComboBoxCount getShownItemCount() const {
    return static_cast<ComboBoxCount>(*(flags & ImGuiComboFlags_::ImGuiComboFlags_HeightMask_));
  }
  /**
   * Set count of items shown when the element is unrolled.
   * @param shownItemCount count of items
   */
  void setShownItemCount(ComboBoxCount shownItemCount) {
    flags &= static_cast<ImGuiComboFlags_>(~ImGuiComboFlags_::ImGuiComboFlags_HeightMask_);
    flags |= static_cast<ImGuiComboFlags_>(shownItemCount);
  }

  /**
   * Close the Combobox in the next render loop.
   */
  void close() { shouldClose = true; }

 protected:
  using AllColorCustomizable::setColorStack;
  using AllStyleCustomizable::setStyleStack;
  void renderImpl() override {
    [[maybe_unused]] auto colorStyle = setColorStack();
    [[maybe_unused]] auto style = setStyleStack();
    const char *previewPtr = previewValue.c_str();
    if (ImGui::BeginCombo(getLabel().c_str(), previewPtr, *flags)) {
      RAII end{ImGui::EndCombo};
      checkClose();
      std::ranges::for_each(CustomItemBox<T, R>::filteredItems, [](auto item) { item->second->render(); });
    }
  }

  /**
   * Call closing impl now.
   */
  void closeNow() { ImGui::CloseCurrentPopup(); }

  /**
   * Check for closing during rendering.
   */
  void checkClose() {
    if (shouldClose) {
      closeNow();
      shouldClose = false;
    }
  }

  Flags<ImGuiComboFlags_> flags{};

 private:
  std::string previewValue{};
  bool shouldClose = false;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMCOMBOBOX_H
