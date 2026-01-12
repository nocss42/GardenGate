cmake_minimum_required(VERSION 3.24)
project(GardenGate VERSION 0.1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED)

# ---- Language / standard ----
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# ---- Sources (project only) ----
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

target_sources(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCES}
)

# ---- Includes ----
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/MinHook"
  "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui"
)

# ---- Warnings ----
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
)

# ---- Libraries ----
if (MINGW)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ws2_32
    "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/MinHook/minhook.a"
  )
elseif (MSVC)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ws2_32.lib
    "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/MinHook/libMinHook-x64-v141-mt.lib"
  )
endif()

# ---- Output ----
set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "dinput8"
)
