cmake_minimum_required(VERSION 3.16)
project(opencv_cpp_basic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(ZBAR REQUIRED zbar)

set(TARGET_NAME 
   55_knndigit )
set(SOURCE_DIR part6)
set(SOURCES ${SOURCE_DIR}/${TARGET_NAME}.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

#target_include_directories(${TARGET_NAME} PRIVATE
#        ${OpenCV_INCLUDE_DIRS}
#        ${ZBAR_INCLUDE_DIRS})

 target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCV_LIBS} ${ZBAR_LIBRARIES})
