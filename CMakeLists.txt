cmake_minimum_required(VERSION 3.17)

include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"
        SHA1 "64024b7b95b4c86d50ae05b926814448c93a70a0"
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

hunter_add_package(imgui)
find_package(imgui CONFIG REQUIRED)
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
CPMAddPackage(
        NAME cppcoro
        GITHUB_REPOSITORY dutow/cppcoro
        GIT_TAG 4b1951ff3c25fea78198a5c0aabb50589946b694
)
include_directories(${toml++_SOURCE_DIR}/include)

set(RANGE_V3_TESTS OFF)
set(SHADERC_SKIP_TESTS ON)
set(SPIRV_CROSS_ENABLE_TESTS OFF)
CPMAddPackage(
        NAME range-v3
        GITHUB_REPOSITORY ericniebler/range-v3
        GIT_TAG 0.11.0
)
CPMAddPackage(
        NAME backward_cpp
        GITHUB_REPOSITORY bombela/backward-cpp
        GIT_TAG v1.5
)

set(SOURCES
        src/imgui/ImGuiInterface.cpp
        src/imgui/elements/Button.cpp
        src/imgui/elements/Checkbox.cpp
        src/imgui/elements/ComboBox.cpp
        src/imgui/elements/Dialog.cpp
        src/imgui/elements/Group.cpp
        src/imgui/elements/Image.cpp
        src/imgui/elements/InputText.cpp
        src/imgui/elements/ListBox.cpp
        src/imgui/elements/Memo.cpp
        src/imgui/elements/MenuBars.cpp
        src/imgui/elements/Panel.cpp
        src/imgui/elements/Plot.cpp
        src/imgui/elements/RadioButton.cpp
        src/imgui/elements/RadioGroup.cpp
        src/imgui/elements/TabBar.cpp
        src/imgui/elements/Text.cpp
        src/imgui/elements/Tree.cpp
        src/imgui/elements/Window.cpp
        src/imgui/elements/interface/Container.cpp
        src/imgui/elements/interface/Element.cpp
        src/imgui/elements/interface/LabeledElement.cpp
        src/imgui/elements/interface/ResizableElement.cpp
        src/imgui/elements/interface/SavableElement.cpp
        )
set(INCLUDES
        src/imgui/elements.h
        src/imgui/ImGuiInterface.h
        src/imgui/serialization.h
        src/imgui/ui_tree_traversal.h
        src/imgui/elements/Button.h
        src/imgui/elements/Checkbox.h
        src/imgui/elements/ColorChooser.h
        src/imgui/elements/ComboBox.h
        src/imgui/elements/Dialog.h
        src/imgui/elements/DragInput.h
        src/imgui/elements/Group.h
        src/imgui/elements/Image.h
        src/imgui/elements/Input.h
        src/imgui/elements/InputText.h
        src/imgui/elements/ListBox.h
        src/imgui/elements/Memo.h
        src/imgui/elements/MenuBars.h
        src/imgui/elements/Panel.h
        src/imgui/elements/Plot.h
        src/imgui/elements/ProgressBar.h
        src/imgui/elements/RadioButton.h
        src/imgui/elements/RadioGroup.h
        src/imgui/elements/Slider.h
        src/imgui/elements/TabBar.h
        src/imgui/elements/Text.h
        src/imgui/elements/Tree.h
        src/imgui/elements/Window.h
        src/imgui/elements/interface/Container.h
        src/imgui/elements/interface/Element.h
        src/imgui/elements/interface/LabeledElement.h
        src/imgui/elements/interface/ResizableElement.h
        src/imgui/elements/interface/SavableElement.h
        src/imgui/elements/interface/ValueObservableElement.h
        )

option(BUILD_SHARED_LIBS "build this library as shared")

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES} ${BACKWARD_ENABLE})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
add_backward(${PROJECT_NAME})

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(${PROJECT_NAME} imgui::imgui pf_common::pf_common cppcoro range-v3)

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