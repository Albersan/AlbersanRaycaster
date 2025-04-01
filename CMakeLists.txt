cmake_minimum_required(VERSION 3.10) # Minimum CMake version
project(AlbersanRaycaster) # Project name

set(CMAKE_CXX_STANDARD 17) # Using C++17

# Add headers
include_directories(${PROJECT_SOURCE_DIR}/libs/SDL2/include)

# Add paths to libs
link_directories(${PROJECT_SOURCE_DIR}/libs/SDL2/lib)

# Define executable
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp) 
add_executable(AlbersanRaycaster ${SRC_FILES})


# Linking SDL2
target_link_libraries(AlbersanRaycaster SDL2 SDL2main SDL2_image SDL2_mixer)
