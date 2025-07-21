cmake_minimum_required(VERSION 3.16)
project(HeatSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/libs/imgui
  ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

file(GLOB IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/libs/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(HeatSim
  ${CMAKE_SOURCE_DIR}/main.cpp
  ${CMAKE_SOURCE_DIR}/heat_cpu.cpp
  ${CMAKE_SOURCE_DIR}/utils/utils.cpp
  ${IMGUI_SOURCES}
)

target_link_libraries(HeatSim
  glfw
  GLEW::GLEW
  ${OPENGL_LIBRARIES}
)
