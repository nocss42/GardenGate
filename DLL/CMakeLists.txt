cmake_minimum_required(VERSION 3.24)
project(GardenGate VERSION 0.1.2 LANGUAGES CXX)

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

FetchContent_Declare(
  minhook
  GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
  GIT_TAG 1e9ad1eb42db11bfcb65461f687c656612d1b555
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.1
)

FetchContent_MakeAvailable(minhook spdlog)

add_library(${PROJECT_NAME} SHARED)

# Language / standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Sources (project only)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

target_sources(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCES}
)

# ============================================================
# Includes
# ============================================================

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
  GIT_HASH="${GIT_HASH}"
  GIT_DATE="${GIT_DATE}"
)

# ============================================================
# Warnings
# ============================================================

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
)

# ============================================================
# Libraries
# ============================================================

target_link_libraries(${PROJECT_NAME} PRIVATE
    ws2_32
    minhook
    spdlog::spdlog
  )

# ============================================================
# Output
# ============================================================

set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "dinput8"
)
