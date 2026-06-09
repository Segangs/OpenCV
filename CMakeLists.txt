cmake_minimum_required(VERSION 3.16)
project(opencv_cpp_basic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

set(TARGET_NAME 
    22_mask )
set(SOURCE_DIR part2)
set(SOURCES ${SOURCE_DIR}/${TARGET_NAME}.cpp)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCV_LIBS})