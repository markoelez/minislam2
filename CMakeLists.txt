cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(MiniSLAM VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-anon-enum-enum-conversion")

# Find packages
find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)

# Include directories for OpenCV and SDL2
include_directories(${OpenCV_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

# Add the executable
file(GLOB SOURCES "minislam2/*.cc")
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${SDL2_LIBRARIES})

# Find all the source and header files for formatting
file(GLOB_RECURSE ALL_SOURCE_FILES
     "${CMAKE_SOURCE_DIR}/minislam2/*.cpp"
     "${CMAKE_SOURCE_DIR}/minislam2/*.cc"
     "${CMAKE_SOURCE_DIR}/minislam2/*.h"
     "${CMAKE_SOURCE_DIR}/minislam2/*.hpp")

# Add a custom target for clang-format
add_custom_target(
    format
    COMMAND clang-format -i ${ALL_SOURCE_FILES}
    COMMENT "Running clang-format on all source and header files"
)