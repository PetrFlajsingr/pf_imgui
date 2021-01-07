//
// Created by petr on 11/8/20.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H

#include "../interface/LabeledElement.h"
#include "../interface/ResizableElement.h"
#include "types/PlotDataBase.h"
#include <bits/ranges_algo.h>
#include <optional>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/_export.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Plot : public LabeledElement, public ResizableElement {
 public:
  Plot(const std::string &elementName, const std::string &caption, std::optional<std::string> xLabel = std::nullopt,
       std::optional<std::string> yLabel = std::nullopt, const ImVec2 &size = ImVec2{-1, 0});

  template<std::derived_from<plot_type::PlotData> T, typename... Args>
  requires std::constructible_from<T, Args...> T &addData(Args &&...args) {
    auto data = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = data.get();
    datas.emplace_back(std::move(data));
    return *ptr;
  }

  template<std::derived_from<plot_type::PlotData> T>
  void dataByName(const std::string &name) {
    if (const auto iter = std::ranges::find_if(datas, [name](const auto &data) { return data->getName() == name; });
        iter != datas.end()) {
      if (auto result = std::dynamic_pointer_cast<T>(*iter); result != nullptr) { return result; }
      throw StackTraceException::fmt("Wrong type for data: '{}' in '{}'", name, getName());
    }
    throw StackTraceException::fmt("Data not found: '{}' in '{}'", name, getName());
  }

  void removeData(const std::string &name);

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> xLabel = std::nullopt;
  std::optional<std::string> yLabel = std::nullopt;

  std::vector<std::unique_ptr<plot_type::PlotData>> datas;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
