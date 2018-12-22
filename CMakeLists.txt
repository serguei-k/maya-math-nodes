# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.

# Define project
cmake_minimum_required(VERSION 3.3)
project(maya-math-nodes VERSION 1.5.0)

# Set project version macro
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# Set default Maya version
if(NOT DEFINED MAYA_VERSION)
    set(MAYA_VERSION 2018)
endif()

# Set node name prefix macro
if(NOT DEFINED NODE_NAME_PREFIX)
    set(NODE_NAME_PREFIX "math_")
endif()
add_definitions(-DNODE_NAME_PREFIX="${NODE_NAME_PREFIX}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Maya REQUIRED)

# Set compile flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_options(/wd4068 /EHsc)
endif()

# Create target
add_library(mayaMathNodes SHARED
    src/Absolute.h
    src/Add.h 
    src/Array.h
    src/Condition.h
    src/Clamp.h
    src/Debug.h
    src/Distance.h
    src/Divide.h
    src/Convert.h
    src/Interpolate.h
    src/Inverse.h
    src/MinMax.h
    src/Multiply.h
    src/Negate.h
    src/Plugin.cpp
    src/Power.h
    src/Round.h
    src/Trig.h
    src/Twist.h
    src/Subtract.h
    src/Utils.h
    src/VectorOps.h)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})

target_link_libraries(mayaMathNodes ${MAYA_LIBRARIES})
set_target_properties(mayaMathNodes PROPERTIES
    COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS}"
    PREFIX ""
    SUFFIX ${MAYA_PLUGIN_EXT})

if(WIN32)
    set_target_properties(mayaMathNodes PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")
endif()

# Set installation directory
if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist)
endif()

configure_file(MayaMathNodes.mod.in MayaMathNodes.mod)

install(TARGETS mayaMathNodes
    LIBRARY DESTINATION ${INSTALL_DIR}/MayaMathNodes/plug-ins/${MAYA_VERSION}
    RUNTIME DESTINATION ${INSTALL_DIR}/MayaMathNodes/plug-ins/${MAYA_VERSION})
install(DIRECTORY python/maya_math_nodes
    DESTINATION ${INSTALL_DIR}/MayaMathNodes/scripts
    PATTERN "*.pyc" EXCLUDE
    PATTERN ".*" EXCLUDE)
install(FILES ${CMAKE_BINARY_DIR}/MayaMathNodes.mod DESTINATION ${INSTALL_DIR})
