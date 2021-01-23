cmake_minimum_required(VERSION 3.17)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.283.tar.gz"
        SHA1 "f445fbb4716e5485e83f6acf110b51d3b8916b2c"
)

project(pf_imgui
        VERSION 0.1.0)

set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
set(CPM_DOWNLOAD_VERSION 0.27.4)

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION} AND CPM_VERSION STREQUAL CPM_DOWNLOAD_VERSION))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

set(CMAKE_CXX_STANDARD 20)
list(APPEND flags "-fconcepts" "-fconcepts-diagnostics-depth=10" "-Werror=return-type" "-fcoroutines"
        "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-Wno-unknown-pragmas" "-Wno-unused-function"
        "-Wpointer-arith" "-Wno-cast-qual" "-Wno-type-limits")
if (CMAKE_BUILD_TYPE MATCHES Release)
    list(APPEND flags "-o3")
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
        GIT_TAG v2.2.0
)

include_directories(src/include/implot)
include_directories(src/include/imgui)

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
        src/pf_imgui/elements/ComboBox.cpp
        src/pf_imgui/elements/Dialog.cpp
        src/pf_imgui/elements/Group.cpp
        src/pf_imgui/elements/Image.cpp
        src/pf_imgui/elements/InputText.cpp
        src/pf_imgui/elements/ListBox.cpp
        src/pf_imgui/elements/Memo.cpp
        src/pf_imgui/elements/MenuBars.cpp
        src/pf_imgui/elements/Panel.cpp
        src/pf_imgui/elements/plots/SimplePlot.cpp
        src/pf_imgui/elements/plots/Plot.cpp
        src/pf_imgui/elements/RadioButton.cpp
        src/pf_imgui/elements/RadioGroup.cpp
        src/pf_imgui/elements/TabBar.cpp
        src/pf_imgui/elements/SliderAngle.cpp
        src/pf_imgui/elements/Text.cpp
        src/pf_imgui/elements/Tree.cpp
        src/pf_imgui/elements/Window.cpp
        src/pf_imgui/elements/Tooltip.cpp
        src/pf_imgui/elements/FileDialog.cpp
        src/pf_imgui/elements/FlameGraph.cpp
        src/pf_imgui/elements/plots/PieChart.cpp
        src/pf_imgui/elements/plots/types/PlotDataBase.cpp
        src/pf_imgui/elements/plots/types/Line.cpp
        src/pf_imgui/elements/plots/types/Stairs.cpp
        src/pf_imgui/elements/plots/types/Shaded.cpp
        src/pf_imgui/elements/plots/types/Stems.cpp
        src/pf_imgui/elements/plots/types/HeatMap.cpp
        src/pf_imgui/elements/plots/types/Digital.cpp
        src/pf_imgui/elements/interface/Container.cpp
        src/pf_imgui/elements/interface/Element.cpp
        src/pf_imgui/elements/interface/LabeledElement.cpp
        src/pf_imgui/elements/interface/ResizableElement.cpp
        src/pf_imgui/elements/interface/SavableElement.cpp
        src/pf_imgui/elements/interface/Clickable.cpp
        src/pf_imgui/elements/interface/ItemElement.cpp
        src/pf_imgui/elements/interface/Focusable.cpp
        src/pf_imgui/elements/interface/Hoverable.cpp
        src/pf_imgui/elements/interface/Collapsible.cpp
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.cpp
        ${IMGUI_FLAME_GRAPH_DIR}/imgui_widget_flamegraph.cpp
        src/include/implot/implot_items.cpp
        src/include/implot/implot.cpp
        src/include/imgui/imgui.cpp
        src/include/imgui/imgui_widgets.cpp
        src/include/imgui/imgui_draw.cpp
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
        src/pf_imgui/elements/ComboBox.h
        src/pf_imgui/elements/Dialog.h
        src/pf_imgui/elements/DragInput.h
        src/pf_imgui/elements/Group.h
        src/pf_imgui/elements/Image.h
        src/pf_imgui/elements/Input.h
        src/pf_imgui/elements/InputText.h
        src/pf_imgui/elements/ListBox.h
        src/pf_imgui/elements/Memo.h
        src/pf_imgui/elements/MenuBars.h
        src/pf_imgui/elements/Panel.h
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
        src/pf_imgui/elements/Window.h
        src/pf_imgui/elements/Tooltip.h
        src/pf_imgui/elements/FileDialog.h
        src/pf_imgui/elements/FlameGraph.h
        src/pf_imgui/elements/plots.h
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
        src/pf_imgui/elements/interface/Container.h
        src/pf_imgui/elements/interface/Element.h
        src/pf_imgui/elements/interface/LabeledElement.h
        src/pf_imgui/elements/interface/ResizableElement.h
        src/pf_imgui/elements/interface/SavableElement.h
        src/pf_imgui/elements/interface/Clickable.h
        src/pf_imgui/elements/interface/ValueObservableElement.h
        src/pf_imgui/elements/interface/ItemElement.h
        src/pf_imgui/elements/interface/Observable_impl.h
        src/pf_imgui/elements/interface/Focusable.h
        src/pf_imgui/elements/interface/Hoverable.h
        src/pf_imgui/elements/interface/Collapsible.h
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.h
        ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialogConfig.h
        ${IMGUI_FLAME_GRAPH_DIR}/imgui_widget_flamegraph.h
        src/pf_imgui/styles/dark.h
        src/include/implot/implot.h
        src/include/implot/implot_internal.h
        src/include/imgui/imgui.h
        src/include/imgui/imgui_internal.h
        src/include/imgui/imconfig.h
        src/include/imgui/imstb_rectpack.h
        src/include/imgui/imstb_textedit.h
        src/include/imgui/imstb_truetype.h
        )

list(APPEND INCLUDES ${toml++_headers})


option(BUILD_SHARED_LIBS "build this library as shared")

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES} ${BACKWARD_ENABLE})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_backward(${PROJECT_NAME})
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
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${toml++_SOURCE_DIR}/include>)
target_link_libraries(${PROJECT_NAME} PUBLIC pf_common::pf_common range-v3)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)


install(TARGETS ${PROJECT_NAME} pf_common EXPORT ${PROJECT_NAME}Targets
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

