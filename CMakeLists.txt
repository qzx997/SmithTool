cmake_minimum_required(VERSION 3.16)

project(SmithTool 
    VERSION 0.1.0
    DESCRIPTION "Interactive Smith Chart Tool for RF Impedance Matching"
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Source files - Core module
set(CORE_SOURCES
    src/core/smithmath.cpp
    src/core/impedance.cpp
    src/core/component.cpp
    src/core/trace.cpp
    src/core/matching.cpp
)

set(CORE_HEADERS
    src/core/smithmath.h
    src/core/impedance.h
    src/core/component.h
    src/core/trace.h
    src/core/matching.h
)

# Source files - Data module
set(DATA_SOURCES
    src/data/sparamdata.cpp
    src/data/touchstone.cpp
    src/data/spiceexporter.cpp
)

set(DATA_HEADERS
    src/data/sparamdata.h
    src/data/touchstone.h
    src/data/spiceexporter.h
)

# Source files - UI module
set(UI_SOURCES
    src/ui/smithchartwidget.cpp
    src/ui/componentpanel.cpp
    src/ui/impedanceinputpanel.cpp
    src/ui/elementtoolbar.cpp
    src/ui/circuitview.cpp
    src/ui/matchingwizard.cpp
    src/ui/componenteditdialog.cpp
    src/ui/mainwindow.cpp
)

set(UI_HEADERS
    src/ui/smithchartwidget.h
    src/ui/componentpanel.h
    src/ui/impedanceinputpanel.h
    src/ui/elementtoolbar.h
    src/ui/circuitview.h
    src/ui/matchingwizard.h
    src/ui/componenteditdialog.h
    src/ui/mainwindow.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Windows application icon
if(WIN32)
    set(WIN_ICON_RC ${CMAKE_CURRENT_SOURCE_DIR}/resources/smithtool.rc)
    if(EXISTS ${WIN_ICON_RC})
        list(APPEND RESOURCES ${WIN_ICON_RC})
    endif()
    # Hide console window in Release mode
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

# All sources
set(ALL_SOURCES
    src/main.cpp
    ${CORE_SOURCES}
    ${DATA_SOURCES}
    ${UI_SOURCES}
)

set(ALL_HEADERS
    ${CORE_HEADERS}
    ${DATA_HEADERS}
    ${UI_HEADERS}
)

# Create executable
add_executable(${PROJECT_NAME}
    ${ALL_SOURCES}
    ${ALL_HEADERS}
    ${RESOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SMITHTOOL_VERSION="${PROJECT_VERSION}"
)

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Optional: Build as library for integration
option(BUILD_SHARED_LIBS "Build as shared library for integration" OFF)

if(BUILD_SHARED_LIBS)
    add_library(SmithToolLib SHARED
        ${CORE_SOURCES}
        ${DATA_SOURCES}
        ${UI_SOURCES}
        ${ALL_HEADERS}
    )
    
    target_include_directories(SmithToolLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    
    target_link_libraries(SmithToolLib PUBLIC
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
    )
    
    # Export headers for integration
    set_target_properties(SmithToolLib PROPERTIES
        PUBLIC_HEADER "${ALL_HEADERS}"
    )
endif()

# Testing (optional)
option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    find_package(Qt6 REQUIRED COMPONENTS Test)
    
    # Add test executables here
    # add_executable(test_smithmath tests/test_smithmath.cpp ${CORE_SOURCES})
    # target_link_libraries(test_smithmath Qt6::Test Qt6::Core)
    # add_test(NAME SmithMathTest COMMAND test_smithmath)
endif()

message(STATUS "SmithTool version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
