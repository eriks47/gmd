cmake_minimum_required(VERSION 3.20)
project(gmd)
enable_testing()

# Options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Dependecies
find_package(GTest CONFIG REQUIRED)

file(GLOB sourcefiles "Source/*.cpp" "Source/*.hpp")
file(GLOB tests "Tests/*.cpp")

add_executable(main ${sourcefiles})

add_executable(tests ${tests} Source/Utillities.hpp Source/Utillities.cpp Source/Header.cpp Source/Header.hpp)
target_link_libraries(tests PRIVATE 
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)
add_test(AllTests tests)
