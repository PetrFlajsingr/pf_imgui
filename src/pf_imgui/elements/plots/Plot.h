/**
 * @file Plot.h
 * @brief Plot element.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
#define PF_IMGUI_ELEMENTS_PLOTS_PLOT_H

#include <bits/ranges_algo.h>
#include <memory>
#include <optional>
#include <pf_common/exceptions/StackTraceException.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <pf_imgui/exceptions.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Advanced plot for plotting 2D data.
 */
class PF_IMGUI_EXPORT Plot : public Element, public Labellable, public Resizable {
 public:
  /**
   * Construct Plot.
   * @param elementName ID of the plot
   * @param label rendered above the plot
   * @param xLabel label of x axis
   * @param yLabel label of y axis
   * @param size size of the element
   */
  Plot(const std::string &elementName, const std::string &label, std::optional<std::string> xLabel = std::nullopt,
       std::optional<std::string> yLabel = std::nullopt, const Size &size = Size::FillWidth());

  /**
   * Add new set of data.
   * @param args argument for data constructor
   * @tparam T type of data storage
   * @tparam Args type of argument for data storage constructor
   * @return reference to he newly created data
   */
  template<std::derived_from<plot_type::PlotData> T, typename... Args>
  requires std::constructible_from<T, Args...> T &addData(Args &&...args) {
    auto data = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = data.get();
    datas.emplace_back(std::move(data));
    return *ptr;
  }

  /**
   * Get data storage by its ID.
   * @param name ID of the data storage
   * @return reference to data storage
   * @throws IdNotFoundException when no such data storage exists
   * @throw StackTraceException when desired type doesn't match the storage data type
   */
  template<std::derived_from<plot_type::PlotData> T>
  T &dataByName(const std::string &name) {
    if (const auto iter = std::ranges::find_if(datas, [name](const auto &data) { return data->getName() == name; });
        iter != datas.end()) {
      if (auto result = dynamic_cast<T>(iter->get()); result != nullptr) { return *result; }
      throw StackTraceException("Wrong type for data: '{}' in '{}'", name, getName());
    }
    throw IdNotFoundException("Data not found: '{}' in '{}'", name, getName());
  }

  /**
   * Remove data storage by ID.
   * @param name ID of the data storage to be removed
   */
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
