cmake_minimum_required(VERSION 3.16)
project(opencv_cpp_basic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(01_helloWorld part1/01_HelloWorld.cpp)
target_link_libraries(01_helloWorld PRIVATE ${OpenCV_LIBS})