cmake_minimum_required(VERSION 3.18)   


project(generator_for_esp32_mqtt)

#add_executable(main main.cpp)
#add_executable(main main2.cpp)

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)