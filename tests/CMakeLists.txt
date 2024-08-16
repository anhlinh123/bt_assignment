find_package(GTest REQUIRED)

add_executable(backend_test backend_test.cpp)
target_compile_features(backend_test PRIVATE cxx_std_20)
target_link_libraries(backend_test PRIVATE gtest_main backend gtest::gtest)

include(GoogleTest)
gtest_discover_tests(backend_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
