cmake_minimum_required(VERSION 3.20)
project(gmd)
enable_testing()

# Options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Dependecies
find_package(GTest CONFIG REQUIRED)

file(GLOB sourcefiles "Source/*.cpp" "Source/*.hpp")
file(GLOB tests "Tests/*.cpp" "Source/*.cpp" "Source/*.hpp")
list(REMOVE_ITEM tests "${CMAKE_SOURCE_DIR}/Source/Main.cpp")

add_executable(main ${sourcefiles})

add_executable(tests ${tests})
target_link_libraries(tests PRIVATE 
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)
add_test(AllTests tests)

add_custom_target(run_tests
    DEPENDS main tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "Running tests"
)

add_custom_target(install_to_bin
    DEPENDS main
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:main> /usr/bin
    COMMENT "Running tests"
)
