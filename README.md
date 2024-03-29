# pf_imgui

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/99e9063c82c943bcb5425edd46173ee5)](https://app.codacy.com/gh/PetrFlajsingr/pf_imgui?utm_source=github.com&utm_medium=referral&utm_content=PetrFlajsingr/pf_imgui&utm_campaign=Badge_Grade_Settings)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/PetrFlajsingr/pf_imgui)](https://github.com/PetrFlajsingr/pf_imgui/releases)
[![ubuntu-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/ubuntu-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)
[![macos-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/macos-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)
[![windows-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/windows-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)

[Documentation](https://petrflajsingr.github.io/pf_imgui/)

This library is a wrapper of [DearImGui](https://github.com/ocornut/imgui). Dependencies are most easily downloaded
via [VCPKG](https://github.com/microsoft/vcpkg), you can
use [my VCPKG registry](https://github.com/PetrFlajsingr/vcpkg-registry) to add this library to your project.

# Examples

In these examples `imgui` is an instance of `std::unique_ptr<ImGuiInterface>`. Examples only show basic usage, the
classes usually have more functionality.

## ImGuiInterface

`ImGuiInterface` needs to be subclassed with a custom rendering backend. It must also provide functionality to update
font atlas when requested.

There are some backend implementations in `pf_imgui/backends`. If you wish to use those you gotta add them to the
project yourself, they are not part of the library by default.

## Config constructors

Most (if not all) elements provide a constructor with a struct as a parameter, which makes it easier to use and makes
the code more readable.

Example creation of `DragInput<int>`:

```cpp
window.createChild<DragInput<int>>("drag_int_1", "Drag int", 1.f, 0.f, 100.f);
```

Example creation of `DragInput<int>` with its config:

```cpp
window.createChild(DragInput<int>::Config{
  .name = "drag_int_1",
  .label = "Drag int",
  .speed = 1.f,
  .min = 0.f,
  .max = 100.f
});
```

## Common functionality

All the functions which register an observer return an instance of `Subscription` which can be used to cancel the
observer.

### Style customization

Many classes allow for changing their style/color via member variables, for example in `Button`:

```cpp
class Button : public ButtonBase, public Labellable, public Resizable {
 public:
  //...
  ColorPalette<ColorOf::Text, ColorOf::TextDisabled, ColorOf::Button, ColorOf::ButtonHovered, ColorOf::ButtonActive,
               ColorOf::NavHighlight, ColorOf::Border, ColorOf::BorderShadow>
      color;
  StyleOptions<StyleOf::FramePadding, StyleOf::FrameRounding, StyleOf::FrameBorderSize, StyleOf::ButtonTextAlign> style;
  Font font = Font::Default();
  // ...
};
auto &button = window.createChild<Button>("button_id", "Click me");
button.color.set<ColorOf::Button>(Color::Red);
button.style.set<StyleOf::FramePadding>(ImVec2{10, 10});
```

### Drag and drop

Many elements provide drag and drop capabilities. For now, elements can only accept a payload of the same type as the
source.

```cpp
auto &sliderSource = imgui->createChild<Slider<float>>(...);
auto &sliderTarget = imgui->createChild<Slider<float>>(...);
sliderSource.setDragAllowed(true);
sliderSource.setDragTooltip("Value: {}"); // if the stored value type is pf::ToStringConvertible, the {} will be replaced with its string representation
sliderTarget.setDropAllowed(true);
```

### Fonts

Only the default font is created upon `ImGuiInterface` creation.

New fonts can be loaded via `FontManager`:

```cpp
auto fontBuilder = imgui->getFontManager().fontBuilder("font name", "path/to/ttf/file");
auto newFont = fontBuilder
                  .setFontSize(16)
                  .addSubfont("path/to/ttf/file") // this allows you to add custom fonts which will be baked inside the master one
                    .setGlyphRange({0xFFFF, 0xFFFF}) // range which will get replaced in the master font
                  .endSubfont()
                  .build();
```

If a font is set to an element it'll be used in its children as well, unless you explicitly set a different font for
them.

```cpp
auto font = imgui->getFontManager().fontByName("arial11");
if (font.has_value()) {
  myTextElement.setFont(font);
}
```

### Clickable

An element which is derived from clickable allows you to add an observer to its click event.

```cpp
auto &button = window.createChild<Button>("button_id", "Click me");
auto subscription = button.clickEvent.addListener([] {
  print("button clicked");
});
// ...
if (something) {
  subscription.unsubscribe(); // cancel the observer
}
```

### Collapsible

Elements derived from this interface allow itself to be collapsed. You can also observe this event.

```cpp
auto &window = imgui->createWindow("window_id", "Window name");
// enables a button to collapse the window, clicking the button makes it collapse and the following callback is called
window.setCollapsible(true); 
window.collapsed.addListener([](bool collapsed) { 
  print("Window collapsed: {}", collapsed); 
});
// ...
window.setCollapsed(true);
```

### ItemElement

ItemElement is an element which can receive focus and hover events. It can also have a tooltip and popup menu.

```cpp
auto &button = window.createChild<Button>("button_id", "Click me");
button.setTooltip("This is a button"); // create a simple text tooltip
button.createTooltip().createChild<Image>(...); // create a tooltip with an image
auto &buttonPopupMenu = button.createOrGetPopupMenu();
buttonPopupMenu.addButonItem("item1", "Remove");
buttonPopupMenu.addButonItem("item2", "Duplicate");
buttonPopupMenu.addCheckboxItem("item3", "Enabled");

button.focused.addListener([] (bool isFocused) {});
button.hovered.addListener([] (bool isHovered) {});
```

### Labellable

An element which provides some sort of label.

```cpp
auto &button = window.createChild<Button>("button_id", "THIS IS A LABEL");
button.setLabel("NEW LABEL");
```

### Positionable

Elements deriving from this interface provide an observable interface for their position. You can also change their
position programmatically.

```cpp
auto &window = imgui->createWindow("window_id", "Window name");
window.position.addListener([](Position newPosition) {});
*window.position.modify() = Position{100, 100};
```

### Resizable

These elements can have both their width and height controlled. Not many elements can actually provide this capability.

```cpp
auto &window = imgui->createWindow("window_id", "Window name");
window.size.addListener([] (Size newSize) {});
*window.size.modify() = Size{Width::Auto(), 100};
```

### Savable

Elements derived from this interface can be serialized to `toml` (and deserialized from it as well). This is used to
save their state to a config file.

```cpp
auto &checkbox = imgui->createChild<Checkbox>("id", "Checkbox", false, Persistent::Yes);
```

## Decorators

### WidthDecorator

This decorator can be applied to elements derived from `ItemElement` to control their width.

```cpp
auto &inputText = imgui->createChild<WidthDecorator<InputText>>("id", Width::Auto(), "Text");
inputText.setWidth(100);
```

## Dialogs

### Window

```cpp
auto &window = imgui->createWindow("window_id", "Window name");
// enables a button to close the window, clicking the button makes the window invisible and the following callback is called
window.setCloseable(true); 
window.closeEvent.addListener([] { 
  print("Window closed"); 
});
// enables a button to collapse the window, clicking the button makes it collapse and the following callback is called
window.setCollapsible(true); 
window.collapsed.addListener([](bool collapsed) { 
  print("Window collapsed: {}", collapsed); 
});
// sets size of the window and calls any listeners
*window.size.modify() = Size{100, 50}; 
// called when setSize is used or size is changed by the user
window.size.addListener([](Size newSize) { 
  print("Window size: {}x{}", newSize.width, newSize.height); 
}); 

// delimit range of allowed window size
window.setMinSizeConstraints(Size{10, 10});
window.setMaxSizeConstraints(Size{100, 100});

// create an element inside the window
window.createChild<Button>("button_id", "Button label"); 

// create or get a MenuBar for the window
auto &menuBar = window.createOrGetMenuBar(); 
```

![img.png](img/window.png)

### ModalDialog

A dialog which blocks user from interacting with anything else.

```cpp
auto &dialog = imgui->getDialogManager().createDialog("dialog_id", "Title");

dialog.createChild<Button>("dialog_close_button", "Close")
    .clickEvent.addListener([&dialog]{
      dialog.close();
    });
// ... add more elements into the dialog

```

![img.png](img/dialog.png)

There are some additional `ModalDialog` descendants for ease of usage:

* `InputDialog` for string input.

```cpp
auto handleInput = [](std::string input) { 
  print(input);
};
auto handleClose = [] { 
  print("closed"); 
};
imgui->getDialogManager().buildInputDialog()
      .title("Title")
      .message("Message to user")
      .onInput(handleInput)
      .onCancel(handleClose)
      .build();
```

* `MessageDialog` to show a message and get user response.

```cpp
// get the button user clicked and return true if you want to close the dialog
auto handleInput = [](ButtonTypes input) { 
  if (input == ButtonTypes::Ok) { return true; } 
}; 
imgui->getDialogManager().buildMessageDialog()
      .title("Title")
      .message("Message to user")
      .buttons(MessageButtons::Ok, MessageButtons::Cancel)
      .onDone(handleInput)
      .build();
```

### FileDialog - uses [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)

Allows the user to select directories or files with a filter.

```cpp
const auto onFilesSelected = [](std::vector<std::filesystem::path> selection) {
  std::ranges::for_each(selection, printPath);
};
const auto onSelectionCanceled = [] { print("Canceled selection"); };

imgui->getDialogManager().buildFileDialog(FileDialogType::File)
      .extension({{"vox", "pf_vox"}, "Voxel model", Color::Red})
      .label("Title")
      .onSelect(onFilesSelected)
      .onCancel(onSelectionCanceled)
      .size(Size{500, 400})
      .startPath("/home/pf_imgui")
      .maxFilesSelected(5)
      .modal() // set as modal
      .build();

```

![img.png](img/filedialog.png)

## Elements

### Button

```cpp
auto &button = window.createChild<Button>("button_id", "Click me");
button.clickEvent.addListener([] {
  print("button clicked");
});
```

![img.png](img/button.png)

### Bullet

A CRTP decorator which adds a bullet to the left of the inner element.

```cpp
auto &button = window.createChild<Bullet<Button>>("button_id", "Click me");
```

![img.png](img/bullet_button.png)

### Checkbox

```cpp
auto &checkbox = window.createChild<Checkbox>("checkbox_id", "Check me", false);
checkbox.checked.addListener([](bool selected) {
  print("Selected: {}", selected);
});
```

![img.png](img/checkbox.png)

### ColorChooser

For selection of colors. It supports 3 or 4 component colors and there are two types available:

* `ColorChooserType::Edit` - slider for each component

* `ColorChooserType::Picker` - slider for each component and a paletter
  ![img.png](img/colorchooser.png)

TODO:

### CustomCombobox

### Combobox

### CustomListbox

### Listbox

### DragInput

### FlameGraph

### Group

### Image

### Input

### InputText

### MarkdownText

### Memo

### MemoryEditor

### Menus

#### details

### ProgressBar

### RadioButton/group

### Range2D

### Selectable

### Separator

### Slider

### Slider2D

### Slider3D

### SliderAngle

### VerticalSlider

### SpinInput

### TabBar

### Table

### StringTable

### Text

### Tooltip

### Tree

## Plots

## Layouts

### AbsoluteLayout

### AnchorLayout

### BoxLayout

### GridLayout

### StackedLayout

### StretchLayout
