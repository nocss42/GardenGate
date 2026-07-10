cmake_minimum_required(VERSION 3.24)
project(GardenGateLauncher VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

# ============================================================
# Get Git commit info
# ============================================================

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

execute_process(
    COMMAND git show -s --format=%cd --date=short
    OUTPUT_VARIABLE GIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT GIT_HASH)
    set(GIT_HASH "unknown")
endif()

if(NOT GIT_DATE)
    set(GIT_DATE "unknown")
endif()

# ============================================================
# Dependencies
# ============================================================

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 934c6a5f5ef2355d6df25395d555cb71f790c4e9
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG 8167d2f64122c3ef7e5a0d9cab239fdba37aa54c
)
FetchContent_MakeAvailable(nlohmann_json)

# xdelta3
FetchContent_Declare(
    xdelta3_src
    GIT_REPOSITORY https://github.com/marco-calautti/xdelta.git
    GIT_TAG ab959ecd9a471a0dec5babc3de9d1e27ca25ffa8
)
FetchContent_MakeAvailable(xdelta3_src)

# ============================================================
# Target
# ============================================================

add_executable(${PROJECT_NAME} WIN32)

if (MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE /MANIFEST:NO)
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ============================================================
# Sources
# ============================================================

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.rc"
)

target_sources(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCES}
)

# ============================================================
# Includes
# ============================================================

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/patcher"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
)

# ============================================================
# Definitions
# ============================================================

target_compile_definitions(${PROJECT_NAME} PRIVATE
    WIN32
    _WINDOWS
    _CRT_SECURE_NO_WARNINGS
    GIT_HASH="${GIT_HASH}"
    GIT_DATE="${GIT_DATE}"
)

# ============================================================
# Warnings
# ============================================================

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /Zc:preprocessor>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
)

# ============================================================
# Link libraries
# ============================================================

target_link_libraries(${PROJECT_NAME} PRIVATE
    imgui
    xdelta3
    nlohmann_json::nlohmann_json

    d3d11
    dxgi
    d3dcompiler
    user32
    gdi32
    shell32
    shlwapi
    ole32
    advapi32
    comctl32
    dwmapi
)

# ============================================================
# Output
# ============================================================

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "GardenGate_Launcher"
)
