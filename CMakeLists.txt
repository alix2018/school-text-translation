cmake_minimum_required(VERSION 2.6)
project(opencv_tests)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

add_executable(opencv_tests main.cpp)
target_link_libraries(opencv_tests ${OpenCV_LIBS})

install(TARGETS opencv_tests RUNTIME DESTINATION bin)
install(DIRECTORY data/ DESTINATION data/)
