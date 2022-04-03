//
// Created by xflajs00 on 03.04.2022.
//

#ifndef PF_IMGUI_GRADIENTEDITOR_H
#define PF_IMGUI_GRADIENTEDITOR_H

#include <imgui_color_gradient.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Focusable.h>
#include <pf_imgui/interface/Hoverable.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueObservable.h>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

namespace pf::ui::ig {

struct GradientPoint {
  Color color;
  float position;
  [[nodiscard]] bool operator==(const GradientPoint &rhs) const;

  [[nodiscard]] static std::optional<GradientPoint> FromToml(const toml::table &src);
  [[nodiscard]] toml::table toToml() const;
};

namespace details {
struct GradientMarkToGradientPoint {
  [[nodiscard]] GradientPoint operator()(ImGradientMark *mark) const;
};
}  // namespace details

using GradientPointsView =
    ranges::transform_view<ranges::ref_view<const std::list<ImGradientMark *>>, details::GradientMarkToGradientPoint>;
struct GradientPointsViewComparator {
  [[nodiscard]] bool operator()(GradientPointsView lhs, GradientPointsView rhs);
};

class GradientEditor : public Element,
                       public ValueObservable<GradientPointsView, GradientPointsViewComparator>,
                       public Hoverable,
                       public Focusable,
                       public Savable {
 public:
  struct Config {
    using Parent = GradientEditor;
    std::string_view name;
    Persistent persistent = Persistent::No;
  };
  explicit GradientEditor(Config &&config);
  explicit GradientEditor(const std::string &name, Persistent persistent = Persistent::No);

  [[nodiscard]] Color getColorAt(float percentage) const;

  void addGradientPoint(GradientPoint gradientPoint);
  void removeGradientPoint(GradientPoint gradientPoint);

 protected:
  void renderImpl() override;

  void unserialize_impl(const toml::table &src) override;
  [[nodiscard]] toml::table serialize_impl() const override;

 private:
  [[nodiscard]] GradientPointsView getPointsView() const;

  ImGradient gradient;
  ImGradientMark *draggingMark = nullptr;
  ImGradientMark *selectedMark = nullptr;
};

}  // namespace pf::ui::ig
#endif  //PF_IMGUI_GRADIENTEDITOR_H
