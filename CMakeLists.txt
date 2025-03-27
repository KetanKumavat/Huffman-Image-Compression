cmake_minimum_required(VERSION 3.10)
project(ImageCompression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(ImageCompression 
    main.cpp
    gui.cpp
    compression.cpp
    huffman.cpp
)

target_link_libraries(ImageCompression PRIVATE 
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    ${OpenCV_LIBS}
)