cmake_minimum_required(VERSION 3.10) 
project(SonOfZaratan VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
