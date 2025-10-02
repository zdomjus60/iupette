cmake_minimum_required(VERSION 3.10)

# Project Name
project(iupette)

# Standard: C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 and SDL2_ttf
find_package(SDL2 REQUIRED)


# Include directory
include_directories(include)

# Library
add_library(iupette SHARED src/iupette.cpp src/Widget.cpp src/RectangleWidget.cpp src/LabelWidget.cpp src/TextBoxWidget.cpp src/ButtonWidget.cpp src/ListBoxWidget.cpp src/ComboBoxWidget.cpp src/Window.cpp)

# Executable
add_executable(iupette_app src/main.cpp)

# Executable for the server application
add_executable(iupette_server server/src/main.cpp)

# Link libraries and include directories
target_include_directories(iupette PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} server/include)

# Link iupette to SDL
target_link_libraries(iupette PRIVATE /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so ${SDL2_LIBRARIES})

# Link the app to iupette AND explicitly to the SDL libs as well
target_link_libraries(iupette_app PRIVATE iupette /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so ${SDL2_LIBRARIES})

# Link the server app
target_include_directories(iupette_server PUBLIC server/include)


