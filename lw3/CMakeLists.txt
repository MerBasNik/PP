cmake_minimum_required(VERSION 4.1)
project(Economy)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
add_library(Economy_lib STATIC
        src/Bank/Bank.cpp
        src/Bank/BankOperationError.cpp
        src/Actors/Actor.cpp
        src/Actors/Homer.cpp
        src/Actors/Marge.cpp
        src/Actors/Bart.cpp
        src/Actors/Lisa.cpp
        src/Actors/Apu.cpp
        src/Actors/MrBurns.cpp
        src/Actors/Chester.cpp
        src/Actors/Nelson.cpp
        src/Actors/Smithers.cpp
        src/Economy/Economy.cpp
)
target_include_directories(Economy_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Economy main.cpp)
target_link_libraries(Economy PRIVATE Economy_lib)



find_program(CLANG_FORMAT clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
add_custom_target(
        format-code
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting all source code with clang-format"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(Google_test_run
        tests/bank/BankTest.cpp
        tests/economy/EconomyTest.cpp
        tests/actors/HomerTest.cpp
        tests/actors/MargeTest.cpp
        tests/actors/BartTest.cpp
        tests/actors/LisaTest.cpp
        tests/actors/MrBurnsTest.cpp
        tests/actors/ApuTest.cpp
        tests/actors/NelsonTest.cpp
        tests/actors/ChesterTest.cpp
        tests/actors/SmithersTest.cpp
)
target_link_libraries(Google_test_run
        GTest::GTest
        GTest::Main
        Economy_lib
)

enable_testing()
add_test(NAME Google_test_run COMMAND Google_test_run)
add_dependencies(Google_test_run format-code)


