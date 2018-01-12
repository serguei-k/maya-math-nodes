# Define project
cmake_minimum_required(VERSION 3.3)
project(maya-math-nodes)

# Set default Maya version
set(MAYA_VERSION 2017)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Maya REQUIRED)

# Set compile flags
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

# Create target
add_library(mayaMathNodes SHARED
    src/Plugin.cpp
    src/Trig.h)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})

target_link_libraries(mayaMathNodes ${MAYA_LIBRARIES})
set_target_properties(mayaMathNodes PROPERTIES
    COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS}"
    PREFIX ""
    SUFFIX ${MAYA_PLUGIN_EXT}
)
