//
// Created by petr on 6/23/21.
//

#include <pf_imgui/dialogs/MessageDialog.h>
#include <pf_imgui/elements/ColorChooser.h>
#include <pf_imgui/elements/Combobox.h>
#include <pf_imgui/elements/DragInput.h>
#include <pf_imgui/elements/Input.h>
#include <pf_imgui/elements/Listbox.h>
#include <pf_imgui/elements/ProgressBar.h>
#include <pf_imgui/elements/Slider.h>
#include <pf_imgui/elements/Slider2D.h>
#include <pf_imgui/elements/Slider3D.h>
#include <pf_imgui/elements/SpinInput.h>
#include <pf_imgui/elements/VerticalSlider.h>

namespace pf::ui::ig {
template class MessageDialog<MessageButtons>;

template class ColorChooser<ColorChooserType::Edit, glm::vec3>;
template class ColorChooser<ColorChooserType::Edit, glm::vec4>;
template class ColorChooser<ColorChooserType::Picker, glm::vec3>;
template class ColorChooser<ColorChooserType::Picker, glm::vec4>;

template class Combobox<std::string>;

template class Listbox<std::string>;

template class DragInput<float>;
template class DragInput<glm::vec2>;
template class DragInput<glm::vec3>;
template class DragInput<glm::vec4>;
template class DragInput<math::Range<float>>;
template class DragInput<int>;
template class DragInput<glm::ivec2>;
template class DragInput<glm::ivec3>;
template class DragInput<glm::ivec4>;
template class DragInput<math::Range<int>>;

template class Input<float>;
template class Input<glm::vec2>;
template class Input<glm::vec3>;
template class Input<glm::vec4>;
template class Input<double>;
template class Input<int>;
template class Input<glm::ivec2>;
template class Input<glm::ivec3>;
template class Input<glm::ivec4>;

template class ProgressBar<float>;
template class ProgressBar<int>;

template class Slider<int>;
template class Slider<glm::ivec2>;
template class Slider<glm::ivec3>;
template class Slider<glm::ivec4>;
template class Slider<float>;
template class Slider<glm::vec2>;
template class Slider<glm::vec3>;
template class Slider<glm::vec4>;

template class Slider2D<int>;
template class Slider2D<float>;

template class Slider3D<float>;

template class SpinInput<int>;
template class SpinInput<float>;

template class VerticalSlider<int>;
template class VerticalSlider<float>;
}// namespace pf::ui::ig