cmake_minimum_required(VERSION 3.0.0)

project(mingw64_cmake VERSION 0.1 LANGUAGES CXX)

set(QTBASE C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/cmake)
set(CMAKE_PREFIX_PATH ${QTBASE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

link_libraries(C:/Qt/Qt5.12.1/5.12.1/mingw73_64/bin)
link_directories(C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib)
include_directories(C:/Qt/Qt5.12.1/5.12.1/mingw73_64/include)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Core.a
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Cored.a
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Gui.a
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Guid.a
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Widgets.a
    C:/Qt/Qt5.12.1/5.12.1/mingw73_64/lib/libQt5Widgetsd.a
)
