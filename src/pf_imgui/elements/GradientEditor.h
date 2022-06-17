/**
 * @file GradientEditor.h
 * @brief Gradient editor.
 * @author Petr Flajšingr
 * @date 3.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_GRADIENTEDITOR_H
#define PF_IMGUI_ELEMENTS_GRADIENTEDITOR_H

#include <pf_imgui/interface/Customizable.h>
#include <imgui_color_gradient.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Focusable.h>
#include <pf_imgui/interface/Hoverable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

namespace pf::ui::ig {

/**
 * @brief Leading point for a gradient.
 */
struct GradientPoint {
  Color color;
  float position; /*!< Position in range [0.0, 1.0] */
  [[nodiscard]] bool operator==(const GradientPoint &rhs) const;

  [[nodiscard]] static std::optional<GradientPoint> FromToml(const toml::table &src);
  [[nodiscard]] toml::table toToml() const;
};

namespace details {
struct GradientMarkToGradientPoint {
  [[nodiscard]] GradientPoint operator()(ImGradientMark *mark) const;
};
}  // namespace details

/**
 * View of gradient points in GradientEditor.
 */
using GradientPointsView =
    ranges::transform_view<ranges::ref_view<const std::list<ImGradientMark *>>, details::GradientMarkToGradientPoint>;
struct GradientPointsViewComparator {
  [[nodiscard]] bool operator()(GradientPointsView lhs, GradientPointsView rhs);
};

// TODO: styles
/**
 * @brief Editor for gradient with color selection and an option to add more leading points. Selected poitns can be removed by pressing delete.
 */
class GradientEditor : public ElementWithID,
                       public ValueObservable<GradientPointsView, GradientPointsViewComparator>,
                       public Hoverable,
                       public Focusable,
                       public Savable,
                       public FontCustomizable {
 public:
  /**
   * @brief Construction args for GradientEditor.
   */
  struct Config {
    using Parent = GradientEditor;
    Explicit<std::string_view> name; /*!< Unique names of the element */
    bool persistent = false;         /*!< Enable disk state saving */
  };
  /**
   * Construct GradientEditor
   * @param config construction args @see GradientEditor::Config
   */
  explicit GradientEditor(Config &&config);
  /**
   * Construct GradientEditor
   * @param name unique name of the element
   * @param persistent enable disk state saving
   */
  explicit GradientEditor(const std::string &name, Persistent persistent = Persistent::No);

  /**
   * Calculate color of gradient at given position.
   * @param percentage value in range [0.0, 1.0]
   * @return color at position
   */
  [[nodiscard]] Color getColorAt(float percentage) const;

  /**
   * Add a new leading point.
   * @param gradientPoint new leading point
   */
  void addGradientPoint(GradientPoint gradientPoint);
  /**
   * Remove leading point.
   */
  void removeGradientPoint(GradientPoint gradientPoint);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

 protected:
  void renderImpl() override;

 private:
  [[nodiscard]] GradientPointsView getPointsView() const;

  ImGradient gradient;
  ImGradientMark *draggingMark = nullptr;
  ImGradientMark *selectedMark = nullptr;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_ELEMENTS_GRADIENTEDITOR_H
