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
#include <pf_imgui/interface/Resizable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

#include <utility>

namespace pf::ui::ig {
namespace details {
/**
 * Default formats for underlying types.
 * @tparam T underlying type
 * @return format
 */
template<typename T>
constexpr const char *defaultVSliderFormat() {
  if constexpr (std::same_as<T, float>) {
    return "%.3f";
  } else {
    return "%d";
  }
}
}// namespace details

/**
 * @brief Slider rendered vertically.
 * @tparam T inner value type
 * @todo: size
 */
template<OneOf<float, int> T>
class PF_IMGUI_EXPORT VerticalSlider : public ItemElement,
                                       public Labellable,
                                       public ValueObservable<T>,
                                       public Savable,
                                       public Resizable,
                                       public DragSource<T>,
                                       public DropTarget<T> {
 public:
  /**
   * Construct VerticalSlider.
   * @param elementName ID of the element
   * @param label text rendered next to the slider
   * @param minVal min allowed value
   * @param maxVal max allowed value
   * @param value starting value
   * @param persistent enable state saving to disk
   * @param format printf-like format for value
   */
  VerticalSlider(const std::string &elementName, const std::string &label, ImVec2 size, T minVal, T maxVal,
                 T value = T{}, Persistent persistent = Persistent::No,
                 std::string format = details::defaultVSliderFormat<T>())
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(value), Savable(persistent),
        Resizable(size), DragSource<T>(false), DropTarget<T>(false), min(minVal), max(maxVal),
        format(std::move(format)) {}

  /**
   * Get min slider value.
   * @return min slider value
   */
  [[nodiscard]] T getMin() const { return min; }
  /**
   * Set min slider value.
   * @param min new min slider value
   */
  void setMin(const T &newMin) { min = newMin; }
  /**
   * Get max slider value.
   * @return max slider value
   */
  [[nodiscard]] T getMax() const { return max; }
  /**
   * Set max slider value.
   * @param max new min slider value
   */
  void setMax(const T &newMax) { max = newMax; }

 protected:
  void renderImpl() override {
    auto valueChanged = false;
    const auto address = ValueObservable<T>::getValueAddress();
    if constexpr (std::same_as<T, float>) {
      valueChanged = ImGui::VSliderFloat(getLabel().c_str(), getSize(), address, min, max, format.c_str());
    }
    if constexpr (std::same_as<T, int>) {
      valueChanged = ImGui::VSliderInt(getLabel().c_str(), getSize(), address, min, max, format.c_str());
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

  toml::table serialize_impl() override { return toml::table{{{"value", ValueObservable<T>::getValue()}}}; }

 private:
  T min;
  T max;
  std::string format;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_VERTICALSLIDER_H