/**
 * @file CustomListBox.h
 * @brief Customizable listbox.
 * @author Petr Flajšingr
 * @date 1.6.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H

#include <algorithm>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/CustomItemBox.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
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
class PF_IMGUI_EXPORT CustomListbox : public CustomItemBox<T, R>, public Labellable, public Resizable {
 public:
  /**
   * Create CustomListbox
   * @param elementName id of the element
   * @param label text rendered next to the element
   * @param rowFactory factory for row creation
   * @param s size of the element
   */
  CustomListbox(const std::string &elementName, const std::string &label, CustomItemBoxFactory<T, R> auto &&rowFactory,
                Size s = Size::Auto())
      : CustomItemBox<T, R>(elementName, std::forward<decltype(rowFactory)>(rowFactory)), Labellable(label),
        Resizable(s) {}

 protected:
  using AllColorCustomizable::setColorStack;
  using AllStyleCustomizable::setStyleStack;
  void renderImpl() override {
    auto colorStyle = setColorStack();
    auto style = setStyleStack();
    if (ImGui::BeginListBox(getLabel().c_str(), getSize().asImVec())) {
      RAII end{[] { ImGui::EndListBox(); }};
      std::ranges::for_each(CustomItemBox<T, R>::filteredItems, [](const auto &item) { item->second->render(); });
    }
  }
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_CUSTOMLISTBOX_H
