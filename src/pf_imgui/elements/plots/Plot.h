/**
 * @file Plot.h
 * @brief Plot element.
 * @author Petr Flajšingr
 * @date 8.11.20
 */

#ifndef PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
#define PF_IMGUI_ELEMENTS_PLOTS_PLOT_H

#include <memory>
#include <optional>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/elements/plots/types/PlotDataBase.h>
#include <pf_imgui/reactive/Observable.h>
#include <string>
#include <utility>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief Advanced plot for plotting 2D data.
 */
class PF_IMGUI_EXPORT Plot : public ElementWithID {
 public:
  /**
   * @brief Struct for construction of Plot.
   */
  struct Config {
    using Parent = Plot;
    Explicit<std::string_view> name;                  /*!< Unique name of the element */
    Explicit<std::string_view> label;                 /*!< Text rendered above the plot */
    std::optional<std::string> xLabel = std::nullopt; /*!< Label of X axis */
    std::optional<std::string> yLabel = std::nullopt; /*!< Label of Y axis */
    Size size = Size::FillWidth();                    /*!< Size of the element */
  };
  /**
   * Construct Plot.
   * @param config construction args @see Plot::Config
   */
  explicit Plot(Config &&config);
  /**
   * Construct Plot.
   * @param elementName ID of the plot
   * @param labelText rendered above the plot
   * @param xLabelText label of x axis
   * @param yLabelText label of y axis
   * @param initialSize size of the element
   */
  Plot(std::string_view elementName, std::string_view labelText, std::optional<std::string> xLabelText = std::nullopt,
       std::optional<std::string> yLabelText = std::nullopt, const Size &initialSize = Size::FillWidth());

  /**
   * Add new set of data.
   * @param args argument for data constructor
   * @tparam T type of data storage
   * @tparam Args type of argument for data storage constructor
   * @return reference to he newly created data
   */
  template<std::derived_from<plot_type::PlotData> T, typename... Args>
    requires std::constructible_from<T, Args...>
  T &addData(Args &&...args) {
    auto data = std::make_unique<T>(std::forward<Args>(args)...);
    const auto ptr = data.get();
    datas.emplace_back(std::move(data));
    return *ptr;
  }

  /**
   * Get data storage by its ID.
   * @param dataName ID of the data storage
   * @return reference to data storage
   * @throw Exception when desired type doesn't match the storage data type
   */
  template<std::derived_from<plot_type::PlotData> T>
  [[nodiscard]] std::optional<std::reference_wrapper<T>> dataByName(const std::string &dataName) {
    if (const auto iter =
            std::ranges::find_if(datas, [dataName](const auto &data) { return data->getName() == dataName; });
        iter != datas.end()) {
      if (auto result = dynamic_cast<T>(iter->get()); result != nullptr) { return *result; }
      return std::nullopt;
    }
    return std::nullopt;
  }

  /**
   * Remove data storage by ID.
   * @param dataName ID of the data storage to be removed
   */
  void removeData(const std::string &dataName);

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  std::optional<std::string> xLabel = std::nullopt;
  std::optional<std::string> yLabel = std::nullopt;

  std::vector<std::unique_ptr<plot_type::PlotData>> datas;
};
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_PLOTS_PLOT_H
