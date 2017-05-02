cmake_minimum_required(VERSION 2.6)
project(bdm)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

set(bdm_FORMS mainwindow.ui)

QT5_WRAP_UI(bdm_FORMS_HEADERS ${bdm_FORMS})

add_executable(bdm
        main.cpp
        BingTranslator.cpp
        BingTranslator.h
        mainwindow.cpp
        mainwindow.h
        ${bdm_FORMS_HEADERS})
target_link_libraries(bdm ${OpenCV_LIBS} Qt5::Core Qt5::Widgets Qt5::Network Qt5::XmlPatterns)

install(TARGETS bdm RUNTIME DESTINATION bin)
install(DIRECTORY data/ DESTINATION data/)
