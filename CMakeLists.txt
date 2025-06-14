cmake_minimum_required(VERSION 3.10)
project(EduWord VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Qt5 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

# Add resources
set(RESOURCES eduword.qrc)

# Add the executable
add_executable(eduword ${SOURCES} ${HEADERS} ${RESOURCES})

# Link libraries
target_link_libraries(eduword Qt5::Widgets Qt5::Core Qt5::Gui Qt5::PrintSupport)

# Install
install(TARGETS eduword DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/eduword.desktop DESTINATION share/applications)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/eduword.png DESTINATION share/icons/hicolor/128x128/apps)