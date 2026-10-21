cmake_minimum_required(VERSION 3.20)
project(Projekt1 LANGUAGES CXX)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

add_executable(Projekt1
        src/main.cpp
        src/Pacjent.cpp
        src/SorManager.cpp
        src/Interface.cpp
)

target_include_directories(Projekt1 PRIVATE include)

target_link_libraries(Projekt1
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)

