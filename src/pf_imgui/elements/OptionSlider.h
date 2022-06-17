/**
 * @file OptionSlider.h
 * @brief OptionSlider element.
 * @author Petr Flajšingr
 * @date 15.6.22
 */

#ifndef PF_IMGUI_ELEMENTS_OPTION_SLIDER
#define PF_IMGUI_ELEMENTS_OPTION_SLIDER

#include <pf_common/concepts/StringConvertible.h>
#include <pf_common/concepts/ranges.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/details/SliderDetails.h>
#include <pf_imgui/interface/DragNDrop.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {

/**
 * @brief A slider class with predefined options.
 * @tparam T type of stored selectable value
 */
template<ToStringConvertible T>
class PF_IMGUI_EXPORT OptionSlider
    : public ItemElement,
      public Labellable,
      public ValueObservable<T>,
      public Savable,
      public DragSource<T>,
      public DropTarget<T>,
      public ColorCustomizable<style::ColorOf::Text, style::ColorOf::TextDisabled, style::ColorOf::DragDropTarget,
                               style::ColorOf::FrameBackground, style::ColorOf::FrameBackgroundHovered,
                               style::ColorOf::FrameBackgroundActive, style::ColorOf::DragDropTarget,
                               style::ColorOf::SliderGrab, style::ColorOf::SliderGrabActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow>,
      public StyleCustomizable<style::Style::FramePadding, style::Style::FrameRounding, style::Style::FrameBorderSize>,
      public FontCustomizable {
 public:
  /**
   * @brief Construction args for OptionSlider /.
   */
  struct Config {
    using Parent = OptionSlider<T>;
    Explicit<std::string> name;      /*!< Unique name of the element */
    Explicit<std::string> label;     /*!< Text rendered next to the element */
    Explicit<std::vector<T>> values; /*!< Possible options */
    bool persistent = false;         /*!< Enable disk state saving */
  };
  /**
   * Construct OptionSlider.
   * @param config construction args @see OptionSlider::Config
   */
  explicit OptionSlider(Config &&config)
      : OptionSlider(config.name, config.label, config.values, config.persistent ? Persistent::Yes : Persistent::No) {}
  /**
   * Construct OptionSlider.
   * @param elementName unique name of the element
   * @param label text rendered next to the element
   * @param newValues possible options
   * @param persistent enable disk state saving
   */
  OptionSlider(const std::string &elementName, const std::string &label, RangeOf<T> auto &&newValues,
               Persistent persistent = Persistent::No)
      : ItemElement(elementName), Labellable(label), ValueObservable<T>(*std::ranges::begin(newValues)),
        Savable(persistent), DragSource<T>(false), DropTarget<T>(false), values{std::ranges::begin(newValues),
                                                                                std::ranges::end(newValues)},
        selectedValueStr(toString(values[0])) {}

  void setValue(const T &newValue) override {
    if (const auto iter =
            std::ranges::find_if(values, [&](const auto &val) { return toString(val) == toString(newValue); });
        iter != values.end()) {
      selectedValueIndex = static_cast<int>(iter - values.begin());
      selectedValueStr = toString(values[selectedValueIndex]);
      ValueObservable<T>::setValue(values[selectedValueIndex]);
    }
  }

  [[nodiscard]] toml::table toToml() const override { return toml::table{{"selected", selectedValueStr}}; }

  void setFromToml(const toml::table &src) override {
    if (auto selectedValIter = src.find("selected"); selectedValIter != src.end()) {
      if (auto selectedVal = selectedValIter->second.as_string(); selectedVal != nullptr) {
        if (const auto iter =
                std::ranges::find_if(values, [&](const auto &val) { return toString(val) == *selectedVal; });
            iter != values.end()) {
          selectedValueIndex = static_cast<int>(iter - values.begin());
          selectedValueStr = toString(values[selectedValueIndex]);
        }
      }
    }
  }

 protected:
  void renderImpl() override {
    [[maybe_unused]] auto colorStyle = setColorStack();
    [[maybe_unused]] auto style = setStyleStack();
    [[maybe_unused]] auto scopedFont = applyFont();
    const auto flags = ImGuiSliderFlags_AlwaysClamp;

    if (ImGui::SliderInt(getLabel().c_str(), &selectedValueIndex, 0, static_cast<int>(values.size() - 1),
                         selectedValueStr.c_str(), flags)) {
      selectedValueStr = toString(values[selectedValueIndex]);
      ValueObservable<T>::setValueAndNotifyIfChanged(values[selectedValueIndex]);
    }

    DragSource<T>::drag(ValueObservable<T>::getValue());
    if (auto drop = DropTarget<T>::dropAccept(); drop.has_value()) {
      ValueObservable<T>::setValueAndNotifyIfChanged(*drop);
    }
  }

 private:
  std::vector<T> values;
  int selectedValueIndex{};
  std::string selectedValueStr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_OPTION_SLIDER
