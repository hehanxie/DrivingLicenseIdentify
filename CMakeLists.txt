cmake_minimum_required(VERSION 3.7)
project(ImageIdentify)

##find_package(OpenCV3)
find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(ImageIdentify ${SOURCE_FILES})
target_link_libraries(ImageIdentify ${OpenCV_LIBS})

find_package( OpenCV REQUIRED )