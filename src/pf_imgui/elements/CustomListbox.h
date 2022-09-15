/**
 * @file CustomListBox.h
 * @brief Customizable listbox.
 * @author Petr Flajšingr
 * @date 1.6.21
 */

#ifndef PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
#define PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomItemBox.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Listbox where rows can be whatever is provided by the user.
 *
 * Rows are created using a factory from stored value.
 * @tparam T type stored in each row
 * @tparam R type stored in each row
 */
template<typename T, std::derived_from<Renderable> R>
class PF_IMGUI_EXPORT CustomListbox : public CustomItemBox<T, R> {
 public:
  /**
   * Create CustomListbox
   * @param elementName id of the element
   * @param labelText text rendered next to the element
   * @param rowFactory factory for row creation
   * @param s size of the element
   */
  CustomListbox(std::string_view elementName, std::string_view labelText, CustomItemBoxFactory<T, R> auto &&rowFactory,
                Size s = Size::Auto());

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;
};

template<typename T, std::derived_from<Renderable> R>
CustomListbox<T, R>::CustomListbox(std::string_view elementName, std::string_view labelText,
                                   CustomItemBoxFactory<T, R> auto &&rowFactory, Size s)
    : CustomItemBox<T, R>(elementName, std::forward<decltype(rowFactory)>(rowFactory)), label(std::string{labelText}),
      size(s) {}

template<typename T, std::derived_from<Renderable> R>
void CustomListbox<T, R>::renderImpl() {
  [[maybe_unused]] auto colorScoped = this->color.applyScoped();
  [[maybe_unused]] auto styleScoped = this->style.applyScoped();
  [[maybe_unused]] auto fontScoped = this->font.applyScopedIfNotDefault();
  if (ImGui::BeginListBox(this->label->get().c_str(), static_cast<ImVec2>(*this->size))) {
    RAII end{ImGui::EndListBox};
    std::ranges::for_each(CustomItemBox<T, R>::filteredItems, [](const auto &item) { item->second->render(); });
  }
}
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
