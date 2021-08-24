cmake_minimum_required(VERSION 3.18..3.20)
project(icu-idn LANGUAGES CXX)

find_package(ICU REQUIRED COMPONENTS uc io)

add_executable(icu-idn main.cpp)
target_link_libraries(icu-idn PRIVATE ICU::uc ICU::io)
