cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(my_project VERSION 0.2.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# enable testing
enable_testing()

# generate a library from sources
add_library(circ_buf STATIC modules/circular_buffer.cpp)
target_include_directories(circ_buf PUBLIC .) # PRIVATE

# as a separate target
add_subdirectory(tests)

add_executable(main_app main.cpp) # app

target_link_libraries(main_app circ_buf)