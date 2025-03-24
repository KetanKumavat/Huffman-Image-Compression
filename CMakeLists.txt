cmake_minimum_required(VERSION 3.10)
project(ImageCompression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt with all required components
find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)
find_package(OpenCV REQUIRED)

# Add resource files if you're using icons
# set(RESOURCE_FILES resources.qrc)
# qt5_add_resources(QT_RESOURCES ${RESOURCE_FILES})

add_executable(ImageCompression 
    main.cpp
    gui.cpp
    compression.cpp
    decompression.cpp
    huffman.cpp
    # ${QT_RESOURCES}  # Uncomment if using resource files
)

target_link_libraries(ImageCompression PRIVATE 
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    ${OpenCV_LIBS}
)