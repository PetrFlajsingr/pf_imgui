/**
 * @file fwd.h
 * @brief Forward declarations.
 * @author Petr Flajšingr
 * @date 4.11.20
 */
#ifndef PF_IMGUI_FWD_H
#define PF_IMGUI_FWD_H
#include <pf_common/concepts/StringConvertible.h>
// TODO: add missing, use where possible to improve compilation times
namespace pf::ui::ig {

class ImGuiInterface;
class FontManager;

class Element;
class ElementContainer;
class Labellable;
class Resizable;
class Savable;

class ResizableLayout;

class BoxLayout;
class AbsoluteLayout;
class GridLayout;
class StackedLayout;
class StretchLayout;

class Button;
class ModalDialog;
class Group;
class Image;
class InputText;
class Memo;
class MenuContainer;
class MenuItem;
class SubMenu;
class WindowMenuBar;
class AppMenuBar;
class MenuBars;
class SimplePlot;
class RadioButton;
class Tab;
class TabBar;
class Text;
class Tree;
class Window;
class Tooltip;
}// namespace pf::ui::ig

#endif//PF_IMGUI_FWD_H
