//
// Created by xflajs00 on 17.09.2022.
//

#include <implot.h>

namespace pf::ui::ig::plot_type::details {
void renderPlotBarsVertical(const char *label, const double *xData, const double *yData, int size, double barSize) {
  ImPlot::PlotBars(label, xData, yData, size, barSize);
}
void renderPlotBarsHorizontal(const char *label, const double *xData, const double *yData, int size, double barSize) {
  ImPlot::PlotBars(label, xData, yData, size, barSize, ImPlotBarsFlags_Horizontal);
}

void renderErrorBarVertical(const char *label, const double *xData, const double *yData, const double *errorData, int size) {
  ImPlot::PlotErrorBars(label, xData, yData, errorData, size);
}
void renderErrorBarHorizontal(const char *label, const double *xData, const double *yData, const double *errorData, int size) {
  ImPlot::PlotErrorBars(label, xData, yData, errorData, size, ImPlotErrorBarsFlags_Horizontal);
}

}  // namespace pf::ui::ig::plot_type::details