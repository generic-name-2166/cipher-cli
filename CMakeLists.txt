cmake_minimum_required(VERSION 3.15)

project(cipher-cli)

find_package(CLI11 REQUIRED)
find_package(cryptopp REQUIRED)

# message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(cipher-cli src/main.cpp)
target_link_libraries(${PROJECT_NAME} CLI11::CLI11)
target_link_libraries(${PROJECT_NAME} cryptopp::cryptopp)
