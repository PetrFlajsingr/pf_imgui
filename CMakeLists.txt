cmake_minimum_required(VERSION 3.17)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.321.tar.gz"
        SHA1 "5e53cbb0429037ea8e2592bfd92704b8ff3ab492"
)

project(pf_imgui
        VERSION 0.7.0)

set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
set(CPM_DOWNLOAD_VERSION 0.32.2)

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION} AND CPM_VERSION STREQUAL CPM_DOWNLOAD_VERSION))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()


include(${CPM_DOWNLOAD_LOCATION})

option(ENABLE_EXTERN_TEMPLATE "enable extern templates" ON)

if (ENABLE_EXTERN_TEMPLATE)
    add_compile_definitions(PF_IMGUI_ENABLE_EXTERN_TEMPLATE)
endif ()

option(ENABLE_STYLES "enable style settings for each element" OFF)

if (ENABLE_STYLES)
    add_compile_definitions(PF_IMGUI_ENABLE_STYLES)
endif ()

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif ()


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND flags "/std:c++latest" "/W4" "/wd4201")
else ()
    list(APPEND flags "-fconcepts" "-fconcepts-diagnostics-depth=10" "-Werror=return-type" "-fcoroutines"
            "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-Wno-unknown-pragmas" "-Wno-unused-function"
            "-Wpointer-arith" "-Wno-cast-qual" "-Wno-type-limits" "-fno-strict-aliasing" "-Wno-format-security")
endif ()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        list(APPEND flags "/WX")
    endif ()
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    list(APPEND flags "-O3")
elseif (CMAKE_BUILD_TYPE MATCHES ASAN)
    list(APPEND flags "-fno-omit-frame-pointer" "-fsanitize=address")
elseif (CMAKE_BUILD_TYPE MATCHES LSAN)
    list(APPEND flags "-fno-omit-frame-pointer" "-fsanitize=leak")
elseif (CMAKE_BUILD_TYPE MATCHES UBSAN)
    list(APPEND flags "-fsanitize=undefined")
elseif (CMAKE_BUILD_TYPE MATCHES TSAN)
    list(APPEND flags "-fsanitize=thread")
endif ()


if (USE_LLD_LINKER)
    list(APPEND flags "-fuse-ld=lld")
elseif (USE_GOLD_LINKER)
    list(APPEND flags "-fuse-ld=gold")
endif ()

set(IMGUI_FILE_DIALOG_DIR src/include/ImGuiFileDialog)
set(IMGUI_FLAME_GRAPH_DIR src/include/ImGuiFlamegraph)


CPMAddPackage(
        NAME pf_common
        GITHUB_REPOSITORY PetrFlajsingr/pf_common
        GIT_TAG master
)

CPMAddPackage(
        NAME toml++
        GITHUB_REPOSITORY marzer/tomlplusplus
        GIT_TAG v3.0.1
)

CPMAddPackage(
        NAME StaticTypeInfo
        GIT_TAG v1.3.1
        GIT_REPOSITORY https://github.com/TheLartians/StaticTypeInfo
)

include_directories(src/include/implot)
include_directories(src/include/imgui)
include_directories(src/include/imgui_markdown)
include_directories(src/include/imgui_memory_editor)
include_directories(src/include/imgInspect)
include_directories(src/include/ImGuiIcons)
include_directories(src/include/imgui_notify)
include_directories(src/include/imgui-command-palette)
include_directories(src/include/imgui_addons/)
include_directories(src/include/imgui-node-editor/)

file(GLOB toml++_headers
        "${toml++_SOURCE_DIR}/include/toml++/*.h"
        "${toml++_SOURCE_DIR}/include/toml++/*.hpp"
        )

include_directories(${toml++_SOURCE_DIR}/include)
include_directories(${IMGUI_FILE_DIALOG_DIR})
include_directories(${IMGUI_FLAME_GRAPH_DIR})

include_directories(${implot_SOURCE_DIR})

set(SOURCES
        src/pf_imgui/ImGuiInterface.cpp
        src/pf_imgui/elements/Button.cpp
        src/pf_imgui/elements/Checkbox.cpp
        src/pf_imgui/dialogs/ModalDialog.cpp
        src/pf_imgui/elements/Expander.cpp
        src/pf_imgui/elements/Image.cpp
        src/pf_imgui/elements/InputText.cpp
        src/pf_imgui/elements/Memo.cpp
        src/pf_imgui/elements/MenuBars.cpp
        src/pf_imgui/elements/plots/SimplePlot.cpp
        src/pf_imgui/elements/plots/Plot.cpp
        src/pf_imgui/elements/RadioButton.cpp
        src/pf_imgui/elements/RadioGroup.cpp
        src/pf_imgui/elements/TabBar.cpp
        src/pf_imgui/elements/SliderAngle.cpp
        src/pf_imgui/elements/Text.cpp
        src/pf_imgui/elements/Tree.cpp
        src/pf_imgui/elements/Separator.cpp
        src/pf_imgui/elements/Selectable.cpp
        src/pf_imgui/elements/Range2D.cpp
        src/pf_imgui/dialogs/Window.cpp
        src/pf_imgui/elements/Tooltip.cpp
        src/pf_imgui/dialogs/FileDialog.cpp
        src/pf_imgui/elements/FlameGraph.cpp
        src/pf_imgui/elements/plots/PieChart.cpp
        src/pf_imgui/elements/plots/types/PlotDataBase.cpp
        src/pf_imgui/elements/plots/types/Line.cpp
        src/pf_imgui/elements/plots/types/Stairs.cpp
        src/pf_imgui/elements/plots/types/Shaded.cpp
        src/pf_imgui/elements/plots/types/Stems.cpp
        src/pf_imgui/elements/plots/types/HeatMap.cpp
        src/pf_imgui/elements/plots/types/Digital.cpp
        src/pf_imgui/elements/MarkdownText.cpp
        src/pf_imgui/interface/ElementContainer.cpp
        src/pf_imgui/interface/Element.cpp
        src/pf_imgui/interface/Labellable.cpp
        src/pf_imgui/interface/Resizable.cpp
        src/pf_imgui/interface/Savable.cpp
        src/pf_imgui/interface/Clickable.cpp
        src/pf_imgui/interface/ItemElement.cpp
        src/pf_imgui/interface/Focusable.cpp
        src/pf_imgui/interface/Hoverable.cpp
        src/pf_imgui/interface/Collapsible.cpp
        src/pf_imgui/interface/Renderable.cpp
        src/pf_imgui/interface/Positionable.cpp
        src/pf_imgui/layouts/ResizableLayout.cpp
        src/pf_imgui/layouts/BoxLayout.cpp
        src/pf_imgui/layouts/GridLayout.cpp
        src/pf_imgui/layouts/StackedLayout.cpp
        src/pf_imgui/layouts/AbsoluteLayout.cpp
        src/pf_imgui/layouts/StretchLayout.cpp
        src/pf_imgui/interface/Layout.cpp
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.cpp
        ${IMGUI_FLAME_GRAPH_DIR}/imgui_widget_flamegraph.cpp
        src/include/implot/implot_items.cpp
        src/include/implot/implot.cpp
        src/include/imgui/imgui.cpp
        src/include/imgui/imgui_widgets.cpp
        src/include/imgui/imgui_draw.cpp
        src/include/imgui/imgui_tables.cpp
        src/include/imgui/imgui_demo.cpp
        src/include/DearWidgets/dear_widgets.cpp
        src/include/imGuIZMO.quat/imGuIZMOquat.cpp
        src/include/ImGuiColorTextEdit/ImGuiColorTextEditor.cpp
        src/include/imgui-command-palette/imcmd_command_palette.cpp
        src/include/imgui-command-palette/imcmd_fuzzy_search.cpp
        src/pf_imgui/interface/DragNDrop.cpp
        src/pf_imgui/elements/MenuItems.cpp
        src/pf_imgui/elements/PopupMenu.cpp
        src/pf_imgui/FontManager.cpp
        src/pf_imgui/layouts/AnchorLayout.cpp
        src/pf_imgui/details/template_instantiations.cpp
        src/pf_imgui/elements/DockSpace.cpp
        src/pf_imgui/details/Spinner.cpp
        src/pf_imgui/elements/Spinner.cpp
        src/pf_imgui/elements/StatusBar.cpp
        src/pf_imgui/dialogs/Notification.cpp
        src/pf_imgui/NotificationManager.cpp
        src/pf_imgui/interface/Closeable.cpp
        src/pf_imgui/details/GroupBox.cpp
        src/pf_imgui/elements/GroupBox.cpp
        src/pf_imgui/elements/TextEditor.cpp
        src/pf_imgui/dialogs/CommandPaletteWindow.cpp
        src/pf_imgui/elements/CommandPalette.cpp
        src/pf_imgui/elements/Toggle.cpp
        src/pf_imgui/layouts/WrapLayout.cpp
        src/pf_imgui/elements/LinkText.cpp
        src/pf_imgui/elements/DatePicker.cpp
        src/include/imgui_addons/imguidatechooser.cpp
        src/include/imgui-node-editor/imgui_node_editor_api.cpp
        src/include/imgui-node-editor/imgui_node_editor.cpp
        src/include/imgui-node-editor/imgui_canvas.cpp
        src/include/imgui-node-editor/crude_json.cpp
        src/pf_imgui/node_editor/Link.cpp
        src/pf_imgui/node_editor/Node.cpp
        src/pf_imgui/node_editor/NodeEditor.cpp
        src/pf_imgui/node_editor/Pin.cpp
        src/pf_imgui/node_editor/blueprint/pins/CheckboxPin.cpp
        src/pf_imgui/node_editor/blueprint/pins/EventPin.cpp
        src/pf_imgui/node_editor/blueprint/pins/ExecPin.cpp
        src/pf_imgui/node_editor/blueprint/pins/PinDraw.cpp
        src/pf_imgui/node_editor/blueprint/pins/StringInputPin.cpp
        src/pf_imgui/node_editor/blueprint/pins/Pin.cpp
        src/pf_imgui/node_editor/blueprint/nodes/SimpleNode.cpp
        src/pf_imgui/node_editor/blueprint/nodes/NodeWithHeader.cpp
        )

set(INCLUDES
        src/pf_imgui/fwd.h
        src/pf_imgui/elements.h
        src/pf_imgui/ImGuiInterface.h
        src/pf_imgui/serialization.h
        src/pf_imgui/ui_tree_traversal.h
        src/pf_imgui/elements/Button.h
        src/pf_imgui/elements/Checkbox.h
        src/pf_imgui/elements/ColorChooser.h
        src/pf_imgui/dialogs/ModalDialog.h
        src/pf_imgui/dialogs/MessageDialog.h
        src/pf_imgui/elements/DragInput.h
        src/pf_imgui/elements/Expander.h
        src/pf_imgui/elements/Image.h
        src/pf_imgui/elements/Input.h
        src/pf_imgui/elements/InputText.h
        src/pf_imgui/elements/Listbox.h
        src/pf_imgui/elements/Memo.h
        src/pf_imgui/elements/MenuBars.h
        src/pf_imgui/elements/plots/SimplePlot.h
        src/pf_imgui/elements/plots/Plot.h
        src/pf_imgui/elements/ProgressBar.h
        src/pf_imgui/elements/RadioButton.h
        src/pf_imgui/elements/RadioGroup.h
        src/pf_imgui/elements/Slider.h
        src/pf_imgui/elements/SliderAngle.h
        src/pf_imgui/elements/TabBar.h
        src/pf_imgui/elements/Text.h
        src/pf_imgui/elements/Tree.h
        src/pf_imgui/elements/Table.h
        src/pf_imgui/elements/Selectable.h
        src/pf_imgui/elements/MarkdownText.h
        src/pf_imgui/dialogs/Window.h
        src/pf_imgui/elements/Tooltip.h
        src/pf_imgui/dialogs/FileDialog.h
        src/pf_imgui/elements/FlameGraph.h
        src/pf_imgui/elements/plots.h
        src/pf_imgui/elements/Separator.h
        src/pf_imgui/elements/Range2D.h
        src/pf_imgui/elements/plots/PieChart.h
        src/pf_imgui/elements/plots/types/PlotDataBase.h
        src/pf_imgui/elements/plots/types/Bar.h
        src/pf_imgui/elements/plots/types/Line.h
        src/pf_imgui/elements/plots/types/Stairs.h
        src/pf_imgui/elements/plots/types/Shaded.h
        src/pf_imgui/elements/plots/types/ErrorBar.h
        src/pf_imgui/elements/plots/types/Stems.h
        src/pf_imgui/elements/plots/types/HeatMap.h
        src/pf_imgui/elements/plots/types/Digital.h
        src/pf_imgui/elements/SpinInput.h
        src/pf_imgui/elements/MemoryEditor.h
        src/pf_imgui/interface/ElementContainer.h
        src/pf_imgui/interface/Element.h
        src/pf_imgui/interface/Labellable.h
        src/pf_imgui/interface/Resizable.h
        src/pf_imgui/interface/Savable.h
        src/pf_imgui/interface/Clickable.h
        src/pf_imgui/interface/ValueObservable.h
        src/pf_imgui/interface/ItemElement.h
        src/pf_imgui/interface/Observable_impl.h
        src/pf_imgui/interface/Focusable.h
        src/pf_imgui/interface/Hoverable.h
        src/pf_imgui/interface/Collapsible.h
        src/pf_imgui/interface/Renderable.h
        src/pf_imgui/interface/Positionable.h
        src/pf_imgui/layouts/ResizableLayout.h
        src/pf_imgui/layouts/BoxLayout.h
        src/pf_imgui/layouts/layouts.h
        src/pf_imgui/layouts/GridLayout.h
        src/pf_imgui/layouts/StackedLayout.h
        src/pf_imgui/layouts/AbsoluteLayout.h
        src/pf_imgui/layouts/StretchLayout.h
        src/pf_imgui/interface/decorators/PositionDecorator.h
        src/pf_imgui/interface/decorators/WidthDecorator.h
        src/pf_imgui/interface/Layout.h
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.h
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialogConfig.h
        ${IMGUI_FLAME_GRAPH_DIR}/imgui_widget_flamegraph.h
        src/pf_imgui/styles/dark.h
        src/include/implot/implot.h
        src/include/implot/implot_internal.h
        src/include/imgui/imgui.h
        src/include/imgui/imgui_internal.h
        src/include/imgui_markdown/imgui_markdown.h
        src/include/imgui/imconfig.h
        src/include/imgui/imstb_rectpack.h
        src/include/imgui/imstb_textedit.h
        src/include/imgui/imstb_truetype.h
        src/pf_imgui/details/ComboFilter.h
        src/pf_imgui/dialogs/InputDialog.h
        src/include/DearWidgets/dear_widgets.h
        src/pf_imgui/elements/Slider2D.h
        src/pf_imgui/unique_id.h
        src/pf_imgui/elements/Slider3D.h
        src/pf_imgui/elements/Bullet.h
        src/pf_imgui/interface/DragNDrop.h
        src/include/imGuIZMO.quat/imGuIZMOquat.h
        src/include/imGuIZMO.quat/vgConfig.h
        src/include/imGuIZMO.quat/vGizmo.h
        src/include/imGuIZMO.quat/vGizmoMath.h
        src/include/imGuIZMO.quat/vgMath.h
        src/pf_imgui/elements/VerticalSlider.h
        src/pf_imgui/elements/CustomListbox.h
        src/pf_imgui/elements/CustomCombobox.h
        src/pf_imgui/elements/Combobox.h
        src/pf_imgui/elements/CustomItemBox.h
        src/pf_imgui/elements/MenuItems.h
        src/pf_imgui/elements/PopupMenu.h
        src/pf_imgui/icons.h
        src/pf_imgui/FontManager.h
        src/pf_imgui/layouts/AnchorLayout.h
        src/pf_imgui/interface/RenderablesContainer.h
        src/pf_imgui/styles/enums.h
        src/pf_imgui/interface/Customizable.h
        src/pf_imgui/elements/DockSpace.h
        src/pf_imgui/details/Spinner.h
        src/pf_imgui/elements/Spinner.h
        src/pf_imgui/elements/StatusBar.h
        src/pf_imgui/dialogs/Notification.h
        src/pf_imgui/NotificationManager.h
        src/pf_imgui/interface/Closeable.h
        src/pf_imgui/details/GroupBox.h
        src/pf_imgui/elements/GroupBox.h
        src/pf_imgui/elements/Gizmo3D.h
        src/include/ImGuiColorTextEdit/ImGuiColorTextEditor.h
        src/pf_imgui/elements/TextEditor.h
        src/include/imgui-command-palette/imcmd_command_palette.h
        src/include/imgui-command-palette/imcmd_fuzzy_search.h
        src/pf_imgui/dialogs/CommandPaletteWindow.h
        src/pf_imgui/elements/CommandPalette.h
        src/pf_imgui/elements/Toggle.h
        src/pf_imgui/layouts/WrapLayout.h
        src/pf_imgui/elements/LinkText.h
        src/pf_imgui/elements/DatePicker.h
        src/include/imgui-node-editor/imgui_node_editor.h
        src/include/imgui-node-editor/imgui_canvas.h
        src/include/imgui-node-editor/crude_json.h
        src/include/imgui-node-editor/imgui_bezier_math.h
        src/include/imgui-node-editor/imgui_extra_math.h
        src/pf_imgui/node_editor/Link.h
        src/pf_imgui/node_editor/Node.h
        src/pf_imgui/node_editor/NodeEditor.h
        src/pf_imgui/node_editor/Pin.h
        src/pf_imgui/node_editor/fwd.h
        src/pf_imgui/node_editor/blueprint/pins/ArrayPin.h
        src/pf_imgui/node_editor/blueprint/pins/CheckboxPin.h
        src/pf_imgui/node_editor/blueprint/pins/DragPin.h
        src/pf_imgui/node_editor/blueprint/pins/PinWithValue.h
        src/pf_imgui/node_editor/blueprint/pins/EventPin.h
        src/pf_imgui/node_editor/blueprint/pins/ExecPin.h
        src/pf_imgui/node_editor/blueprint/pins/InteractablePin.h
        src/pf_imgui/node_editor/blueprint/pins/PinDraw.h
        src/pf_imgui/node_editor/blueprint/pins/StringInputPin.h
        src/pf_imgui/node_editor/blueprint/pins/ValuePin.h
        src/pf_imgui/node_editor/blueprint/pins/Pin.h
        src/pf_imgui/node_editor/blueprint/nodes/SimpleNode.h
        src/pf_imgui/node_editor/blueprint/nodes/NodeWithHeader.h
        )

list(APPEND INCLUDES ${toml++_headers})


option(BUILD_SHARED_LIBS "build this library as shared" OFF)
option(BUILD_DOCS "build doxygen docs" OFF)

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE ${flags})

if (MEASURE_BUILD_TIME)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif ()

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/implot>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/ImGuiFileDialog>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/ImGuiFileFlameGraph>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui_markdown>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imGuIZMO.quat>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/ImGuiFlamegraph>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui_memory_editor>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgInspect>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/ImGuiIcons>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/ImGuiColorTextEdit>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui-command-palette>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui_addons>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui-node-editor>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${toml++_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT_NAME} PUBLIC pf_common range-v3 StaticTypeInfo magic_enum)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)


install(TARGETS ${PROJECT_NAME} pf_common magic_enum StaticTypeInfo
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(
        FILES
        ${INCLUDES}
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT
        Devel
)

install(
        FILES
        ${GENERATED_INCLUDES}
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Generated
        COMPONENT
        Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

export(EXPORT ${PROJECT_NAME}Targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        )

set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}Targets
        FILE
        ${PROJECT_NAME}Targets.cmake
        NAMESPACE
        ${PROJECT_NAME}::
        DESTINATION
        ${ConfigPackageLocation}
        )

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
        "include($" "{CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)"
        )

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
)


if (BUILD_DOCS)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(HTML_DYNAMIC_MENUS YES)
        doxygen_add_docs(pf_imgui_doc ALL
                ${CMAKE_SOURCE_DIR}/src/pf_imgui
                )
    endif ()

endif ()
