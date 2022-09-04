/**
 * @file MatrixDragInput.h
 * @brief MatrixDragInput element.
 * @author Petr Flajšingr
 * @date 23.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_MATRIXDRAGINPUT_H
#define PF_IMGUI_ELEMENTS_MATRIXDRAGINPUT_H

#include <glm/glm.hpp>
#include <pf_common/Explicit.h>
#include <pf_imgui/elements/details/DragInputDetails.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/interface/ValueObservable.h>

namespace pf::ui::ig {
#define PF_IMGUI_GLM_MAT_TYPES                                                                                         \
  glm::mat2, glm::mat3, glm::mat4, glm::mat2x3, glm::mat2x4, glm::mat3x2, glm::mat3x4, glm::mat4x2, glm::mat4x3

/**
 * @brief Drag input element for glm matrices.
 * @tparam M Matrix type
 */
template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
class MatrixDragInput : public ItemElement, public ValueObservable<M>, public Savable {
 public:
  using ParamType = drag_details::UnderlyingType<typename M::col_type>;
  constexpr static auto Height = M::length();
  constexpr static auto Width = M::col_type::length();
  /**
   * @brief Construction args.
   */
  struct Config {
    using Parent = MatrixDragInput;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the element */
    Explicit<ParamType> speed;        /*!< Value change speed */
    Explicit<ParamType> min;          /*!< Minimum allowed value for each cell */
    Explicit<ParamType> max;          /*!< Maximum allowed value for each cell */
    M value = M{};                    /*!< Initial value */
    bool persistent = false;          /*!< Enable state saving */
  };
  /**
   * Construct MatrixDragInput.
   * @param config construction args @see MatrixDragInput::Config
   */
  explicit MatrixDragInput(Config &&config);
  /**
   * Construct MatrixDragInput.
   * @param name unique name of the element
   * @param label text rendered next to the element
   * @param speed value change speed
   * @param min minimum allowed value for each cell
   * @param max maximum allowed value for each cell
   * @param value initial value
   * @param persistent enable state saving
   */
  MatrixDragInput(const std::string &name, const std::string &label, ParamType changeSpeed, ParamType minVal,
                  ParamType maxVal, M initValue, Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::DragDropTarget, ColorOf::FrameBackground,
               ColorOf::FrameBackgroundHovered, ColorOf::FrameBackgroundActive, ColorOf::NavHighlight, ColorOf::Border,
               ColorOf::BorderShadow, ColorOf::SliderGrab, ColorOf::SliderGrabActive>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize> style;
  Font font = Font::Default();
  Observable<Label> label;

 protected:
  void renderImpl() override;

 private:
  std::array<std::string, Height - 1> dragNames;
  float min;
  float max;
  float speed;
};

template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
MatrixDragInput<M>::MatrixDragInput(MatrixDragInput::Config &&config)
    : MatrixDragInput(std::string{config.name.value}, std::string{config.label.value}, config.speed, config.min,
                      config.max, config.value, config.persistent ? Persistent::Yes : Persistent::No) {}

template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
MatrixDragInput<M>::MatrixDragInput(const std::string &name, const std::string &label,
                                    MatrixDragInput::ParamType changeSpeed, MatrixDragInput::ParamType minVal,
                                    MatrixDragInput::ParamType maxVal, M initValue, Persistent persistent)
    : ItemElement(name), ValueObservable<M>(initValue), Savable(persistent), label(label), min(minVal), max(maxVal),
      speed(changeSpeed) {
  for (std::size_t i = 0; i < Height - 1; ++i) { dragNames[i] = std::string{"##drag_"} + std::to_string(i); }
}

template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
toml::table MatrixDragInput<M>::toToml() const {
  return toml::table{{"value", serializeGlmMat(ValueObservable<M>::getValue())}};
}

template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
void MatrixDragInput<M>::setFromToml(const toml::table &src) {
  if (auto newValIter = src.find("value"); newValIter != src.end()) {
    if (auto newVal = newValIter->second.as_array(); newVal != nullptr) {
      if (const auto matValue = safeDeserializeGlmMat<M>(*newVal); matValue.has_value()) {
        ValueObservable<M>::setValueAndNotifyIfChanged(*matValue);
      }
    }
  }
}

template<OneOf<PF_IMGUI_GLM_MAT_TYPES> M>
void MatrixDragInput<M>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  ImGui::BeginGroup();
  if (ImGui::BeginTable("lay", 1)) {
    auto valueChanged = false;
    const auto firstDragName = label->get() + "##drag";  // TODO: cache this
    for (typename M::length_type row = 0; row < Height; ++row) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      const char *dragName = firstDragName.c_str();
      if (row > 0) { dragName = dragNames[row - 1].c_str(); }

      const auto rowValueChanged =
          ImGui::DragScalarN(dragName, ImGuiDataType_Float,
                             glm::value_ptr((*ValueObservable<M>::getValueAddress())[row]), Width, speed, &min, &max);

      valueChanged = valueChanged || rowValueChanged;
    }
    if (valueChanged) { ValueObservable<M>::notifyValueChanged(); }
    ImGui::EndTable();
  }
  ImGui::EndGroup();
}

}  // namespace pf::ui::ig

#endif