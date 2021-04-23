//
// Created by petr on 4/23/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_VERTICALSLIDER_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_VERTICALSLIDER_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {
namespace details {
/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultVSliderFormat() {
  if constexpr (std::same_as < T, float >>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}// namespace details

template<OneOf<float, int> T>
class PF_IMGUI_EXPORT VerticalSlider : public ItemElement,
                                       public Labellable,
                                       public ValueObservable<T>,
                                       public Savable,
                                       public DragSource<T>,
                                       public DropTarget<T> {
 public:
  VerticalSlider(const std::string &elementName, const std::string &label, T minVal, const T maxVal, T value = T{},
                 Persistent persistent = Persistent::No, const std::string &format = defaultVSliderFormat<T>())
      : ItemElement(elementName), Labellable(label), ValueObservable(value), Savable(persistent), DragSource(false),
        DropTarget(false), min(minVal), max(maxVal), format(format) {}

 protected:
  void renderImpl() override {
    auto valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::VSliderFloat(getLabel().c_str(), address, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::VSliderInt(getLabel().c_str(), address, min, max, format.c_str());
    }
    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
      return;
    }
    if (valueChanged) { ValueObservable<T>::notifyValueChanged(); }
  }

  void unserialize_impl(const toml::table &src) override {
    ValueObservable<T>::setValueAndNotifyIfChanged(*src["value"].value<T>());
  }

  toml::table serialize_impl() override { return toml::table{{{"value", value}}}; }

 private:
  T min;
  T max;
  std::string format;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_VERTICALSLIDER_H
