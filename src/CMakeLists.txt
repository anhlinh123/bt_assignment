find_package(Drogon REQUIRED)

add_library(backend OBJECT backend.cpp controller.cpp)
target_include_directories(backend PUBLIC ../include)
target_compile_features(backend PUBLIC cxx_std_20)
target_link_libraries(backend JsonCpp::JsonCpp Drogon::Drogon)
