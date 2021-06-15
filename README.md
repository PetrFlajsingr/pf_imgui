# pf_imgui
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/99e9063c82c943bcb5425edd46173ee5)](https://app.codacy.com/gh/PetrFlajsingr/pf_imgui?utm_source=github.com&utm_medium=referral&utm_content=PetrFlajsingr/pf_imgui&utm_campaign=Badge_Grade_Settings)
[![Build Status](https://travis-ci.org/PetrFlajsingr/pf_imgui.svg?branch=master)](https://travis-ci.org/PetrFlajsingr/pf_imgui)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/PetrFlajsingr/pf_imgui)](https://github.com/PetrFlajsingr/pf_imgui/releases)
[![ubuntu-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/ubuntu-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)
[![macos-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/macos-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)
[![windows-build Actions Status](https://github.com/PetrFlajsingr/pf_imgui/workflows/windows-build/badge.svg)](https://github.com/PetrFlajsingr/pf_imgui/actions)


[Documentation](https://petrflajsingr.github.io/pf_imgui/)

This library is a wrapper around [DearImGui](https://github.com/ocornut/imgui).

# Examples
In these examples `imgui` is an instance of `std::unique_ptr<ImGuiInterface>`. Examples only show basic usage, the classes usually have more functionality.

### ImGuiInterface
`ImGuiInterface` needs to be subclassed with a custom rendering backend. It must also provide functionality to update font atlas when requested. This request is marked via a `protected` member variable `shouldUpdateFontAtlas`.

### Window
```cpp
auto &window = imgui->createWindow("window_id", "Window name");
// enables a button to close the window, clicking the button makes the window invisible and the following callback is called
window.setCloseable(true); 
window.addCloseListener([] { 
  print("Window closed"); 
});
// enables a button to collapse the window, clicking the button makes it collapse and the following callback is called
window.setCollapsible(true); 
window.addCollapseListener([](bool collapsed) { 
  print("Window collapsed: {}", collapsed); 
});
// sets size of the window and calls any listeners
window.setSize(Size{100, 50}); 
// called when setSize is used or size is changed by the user
window.addSizeListener([](Size newSize) { 
  print("Window size: {}x{}", newSize.width, newSize.height); 
}); 

// delimit range of allowed window size
window.setMinSizeConstraints(Size{10, 10});
window.setMaxSizeConstraints(Size{100, 100});

// create an element inside the window
window.createChild<Button>("button_id", "Button label"); 

// create or get a MenuBar for the window
auto &menuBar = window.getMenuBar(); 
```
![img.png](img/window.png)

### ModalDialog
A dialog which blocks user from interacting with anything else.
```cpp
auto &dialog = imgui->createDialog("dialog_id", "Title");

dialog.createChild<Button>("dialog_close_button", "Close")
    .addClickListener([&dialog] {
      dialog.close();
    });
// ... add more elements into the dialog

```
![img.png](img/dialog.png)

There are some additional `ModalDialog` descendants for easy of usage:

* `InputDialog` for string input.
```cpp
auto handleInput = [](std::string input) { 
  print(input);
};
auto handleClose = [] { 
  print("closed"); 
};
auto &inputDialog = imgui->openInputDialog("Title", 
                                           "message to user", 
                                           handleInput, 
                                           handleClose);
```

* `MessageDialog` to show a message and get user response.
```cpp
// get the button user clicked and return true if you want to close the dialog
auto handleInput = [](ButtonTypes input) { 
  if (input == ButtonTypes::Ok) { return true; } 
}; 
using namespace pf::enum_operators;
auto &inputDialog = imgui->createMsgDlg("Title", 
                                        "message to user", 
                                        ButtonTypes::Ok | ButtonTypes::Cancel, 
                                        handleInput);
```


### FileDialog - uses [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)
Allows the user to select directories or files with a filter.
```cpp
const auto allowedExtensions = {"vox", "pf_vox"};
const auto filetypeDescription = "Vox model";
const auto filetypeColor = ImVec4{1, 0, 0, 1};
const auto extensionSettings = FileExtensionSettings{allowedExtensions, 
                                                     filetypeDescription, 
                                                     filetypeColor};
// there can be multiple of these settings
const auto onFilesSelected = [](std::vector<std::filesystem::path> selection) {
  std::ranges::for_each(selection, printPath);
};
const auto onSelectionCanceled = [] { print("Canceled selection"); };
const auto startPath = "/home/pf_imgui"; // path at which the dialog will open
imgui->openFileDialog("Title", 
                      {extensionSettings}, 
                      onFilesSelected, 
                      onSelectionCanceled, 
                      Size{500, 400}, 
                      startPath);
```
![img.png](img/filedialog.png)

### Button
```cpp
auto &button = window.createChild<Button>("button_id", "Click me");
button.addClickListener([] {
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