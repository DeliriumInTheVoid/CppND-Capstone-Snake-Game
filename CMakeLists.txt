cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

file(GLOB_RECURSE SOURCES "./sources/*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/headers)

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable (SnakeGame ${SOURCES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(SnakeGame
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    imgui::imgui
)
