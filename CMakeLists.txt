cmake_minimum_required(VERSION 3.10)

project(minislam VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-anon-enum-enum-conversion")

find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "minislam2/*.cc")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${SDL2_LIBRARIES})

file(GLOB_RECURSE ALL_SOURCE_FILES
     "${CMAKE_SOURCE_DIR}/minislam2/*.cpp"
     "${CMAKE_SOURCE_DIR}/minislam2/*.cc"
     "${CMAKE_SOURCE_DIR}/minislam2/*.h"
     "${CMAKE_SOURCE_DIR}/minislam2/*.hpp")

add_custom_target(
    format
    COMMAND clang-format -i ${ALL_SOURCE_FILES}
    COMMENT "Running clang-format on all source and header files"
)