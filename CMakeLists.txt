cmake_minimum_required(VERSION 3.7)
project(DriverLicenseIdentify)

##find_package(OpenCV3)
find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RedMarkArea.cpp RedMarkArea.h DrivingLicense.cpp DrivingLicense.h)
set(PYTHON_FILES Tesseract.py)
add_executable(DriverLicenseIdentify ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(DriverLicenseIdentify ${OpenCV_LIBS} ${PYTHON_LIBRARIES})

find_package( OpenCV REQUIRED )

