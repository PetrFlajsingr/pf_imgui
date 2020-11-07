//
// Created by petr on 11/4/20.
//

#ifndef PF_IMGUI_ENUMS_H
#define PF_IMGUI_ENUMS_H

namespace pf::ui::ig {

enum class Visibility {
  Visible,
  Invisible
};

enum class Modal { Yes,
                   No };

enum class TextInputType { SingleLine,
                           MultiLine };

enum class PlotType { Lines,
                      Histogram };

enum class ButtonType { Normal,
                        Small,
                        ArrowUp,
                        ArrowLeft,
                        ArrowRight,
                        ArrowDown };

enum class Persistent {
  Yes,
  No
};

enum class ColorChooserType { Edit,
                              Picker };

enum class PanelLayout { Vertical,
                         Horizontal };
}// namespace pf::ui::ig

#endif//PF_IMGUI_ENUMS_H
