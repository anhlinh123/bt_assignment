cmake_minimum_required(VERSION 3.28.3)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(Drogon REQUIRED)

enable_testing()

add_library(backend STATIC src/backend.cpp)
target_link_libraries(backend JsonCpp::JsonCpp)
add_executable(server src/server.cpp)
target_link_libraries(server backend Drogon::Drogon)

#include(GoogleTest)
#gtest_discover_tests(backend_test)
