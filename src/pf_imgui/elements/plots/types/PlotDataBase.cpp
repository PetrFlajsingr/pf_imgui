//
// Created by petr on 11/8/20.
//

#include "PlotDataBase.h"
#include <string>

namespace pf::ui::ig::plot_type {

LabeledPlotData::LabeledPlotData(std::string_view elementName, std::string_view caption)
    : PlotData(elementName), label(std::string{caption}) {}

PlotData::PlotData(std::string_view elementName) : ElementWithID(elementName) {}

}  // namespace pf::ui::ig::plot_type
