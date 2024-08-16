cmake_minimum_required(VERSION 3.28.3)
project(server)

add_subdirectory(src)

add_executable(server main.cpp)
target_compile_features(server PRIVATE cxx_std_20)
target_link_libraries(server PRIVATE backend)

enable_testing()
add_subdirectory(tests)

