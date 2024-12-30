cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(my_project VERSION 0.2.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_library(gameoflife STATIC modules/Keeper.cpp 
                              modules/Calculator.cpp
                              modules/ArgumentParser.cpp
                              modules/WReader.cpp
                              modules/Interface.cpp)

add_subdirectory(tests)

add_executable(main_app main.cpp)

target_link_libraries(main_app gameoflife)
