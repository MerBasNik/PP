cmake_minimum_required(VERSION 4.1)
project(Economy)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
add_library(Economy_lib STATIC
        src/Bank/Bank.cpp
        src/Wallet/Wallet.cpp
        src/Args/Args.cpp
        src/Actors/Actor.cpp
        src/Actors/Customer.cpp
        src/Actors/Homer.cpp
        src/Actors/Marge.cpp
        src/Actors/Bart.cpp
        src/Actors/Lisa.cpp
        src/Actors/Apu.cpp
        src/Actors/MrBurns.cpp
        src/Economy/Economy.cpp
)
target_include_directories(Economy_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Economy_lib PUBLIC Threads::Threads)

add_executable(bank main.cpp)
target_link_libraries(bank PRIVATE Economy_lib)



find_program(CLANG_FORMAT clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
if (CLANG_FORMAT)
    add_custom_target(
            format-code
            COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
            COMMENT "Formatting all source code with clang-format"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif ()


add_executable(Google_test_run
        tests/bank/BankTest.cpp
        tests/wallet/WalletTest.cpp
        tests/args/ArgsTest.cpp
        tests/economy/EconomyTest.cpp
        tests/actors/HomerTest.cpp
        tests/actors/MargeTest.cpp
        tests/actors/BartTest.cpp
        tests/actors/LisaTest.cpp
        tests/actors/MrBurnsTest.cpp
        tests/actors/ApuTest.cpp
)
target_link_libraries(Google_test_run
        GTest::gtest
        GTest::gtest_main
        Economy_lib
)

enable_testing()
add_test(NAME Google_test_run COMMAND Google_test_run)
if (CLANG_FORMAT)
    add_dependencies(Google_test_run format-code)
endif ()
