cmake_minimum_required(VERSION 3.10)

project(CHIP8Emu VERSION 1.0)

find_package(SDL2 REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp gui/gui.cpp emulator/chip8.cpp emulator/log.cpp)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} Threads::Threads)