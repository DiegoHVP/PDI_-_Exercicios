cmake_minimum_required(VERSION 3.0.0)
project(fillLocals VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(fillLocals fillLocals.cpp)
target_link_libraries(fillLocals ${OpenCV_LIBS})