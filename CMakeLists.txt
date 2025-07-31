cmake_minimum_required(VERSION 3.10)
project(multithreading_and_tasks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
if(MSVC)
    target_compile_options(main PRIVATE /W4 /O2)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -pthread -O2)
endif()