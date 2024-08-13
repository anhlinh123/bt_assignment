cmake_minimum_required(VERSION 3.28.3)
project(backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(Drogon REQUIRED)
find_package(jsoncpp REQUIRED)

enable_testing()

add_library(database STATIC database.cpp)
target_link_libraries(database JsonCpp::JsonCpp)
add_executable(backend backend.cpp)
target_link_libraries(backend database Drogon::Drogon)

#include(GoogleTest)
#gtest_discover_tests(backend_test)
