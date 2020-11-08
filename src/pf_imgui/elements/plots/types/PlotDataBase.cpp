//
// Created by petr on 11/8/20.
//

#include "PlotDataBase.h"

namespace pf::ui::ig::plot_type {
PlotData::PlotData(const std::string &elementName) : Element(elementName) {}

LabeledPlotData::LabeledPlotData(const std::string &elementName, const std::string &caption)
    : Element(elementName), PlotData(elementName), LabeledElement(elementName, caption) {}
}// namespace pf::ui::ig::plot_type