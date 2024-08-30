cmake_minimum_required (VERSION 3.20)

project ("ImGuiInstance")

add_library(imgui_instance INTERFACE)

target_include_directories(imgui_instance INTERFACE src)

find_package(glad CONFIG REQUIRED)
target_link_libraries(imgui_instance INTERFACE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(imgui_instance INTERFACE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(imgui_instance INTERFACE imgui::imgui)

find_package(implot CONFIG REQUIRED)
target_link_libraries(imgui_instance INTERFACE implot::implot)

add_library(imgui_instance::imgui_instance ALIAS imgui_instance)
