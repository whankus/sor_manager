cmake_minimum_required(VERSION 3.20)
project(Projekt1 LANGUAGES CXX)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# --- FTXUI (interfejs terminalowy) ---
# Przypieta wersja, zeby projekt budowal sie tak samo za rok.
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# --- Google Test ---
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Aplikacja ---
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

# --- Testy jednostkowe ---
enable_testing()

add_executable(Testy
        tests/SmartArrayTests.cpp
        tests/SorManagerTests.cpp
        src/Pacjent.cpp
        src/SorManager.cpp
)

target_include_directories(Testy PRIVATE include)
target_link_libraries(Testy PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Testy)
