/**
 * @file fwd.h
 * @brief Forward declarations.
 * @author Petr Flajšingr
 * @date 4.11.20
 */
#ifndef PF_IMGUI_FWD_H
#define PF_IMGUI_FWD_H

// TODO: add missing, use where possible to improve compilation times
// TODO: add templated classes
namespace pf::ui::ig {

class ImGuiInterface;
class FontManager;
class DialogManager;
class NotificationManager;

class Element;
class ElementWithID;
class ElementContainer;
class Savable;
class DragNDrop;
class ItemElement;
class Layout;
class Renderable;
class RenderablesContainer;
class TomlSerializable;

class AbsoluteLayout;
class GridLayout;
class StackedLayout;
class StretchLayout;

class BackgroundDockingArea;
class CommandPaletteWindow;
class FileDialog;
class InputDialog;
class MessageDialog;
class ModalDialog;
class Notification;
class Window;

class AppMenuBar;
class AppStatusBar;
class Button;
class Checkbox;
class CommandPalette;
class ConsolePanel;
class DatePicker;
class DockSpace;
class DragNDropGroup;
class Expander;
class Dummy;
class FlameGraph;
class GradientEditor;
class GroupBox;
class Image;
class InspectableImage;
class IndeterminateProgressBar;
class InputText;
class LinkText;
class MarkdownText;
class MemoryEditor;
class OverlayGizmo;
class PopupMenu;
class RadioButton;
class RadioGroup;
class Range2D;
class Separator;
class SimpleCurveEditor;
class SliderAngle;
class Spinner;
class StatusBar;
class TabBar;
class Text;
class TextEditor;
class TimePicker;
class Toggle;
class Tooltip;
class ViewportOverlayGizmo;
class WindowMenuBar;
}  // namespace pf::ui::ig

#endif  // PF_IMGUI_FWD_H
