cmake_minimum_required(VERSION 2.6)
project(opencv_tests)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

add_executable(opencv_tests main.cpp BingTranslator.cpp BingTranslator.h)
target_link_libraries(opencv_tests ${OpenCV_LIBS} Qt5::Core Qt5::Widgets Qt5::Network Qt5::XmlPatterns)

install(TARGETS opencv_tests RUNTIME DESTINATION bin)
install(DIRECTORY data/ DESTINATION data/)
