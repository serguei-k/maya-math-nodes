# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.

if(APPLE)
    set(MAYA_PLUGIN_EXT .bundle)
    set(MAYA_COMPILE_DEFINITIONS "OSMac_;MAC_PLUGIN")
    
    if(NOT DEFINED MAYA_LOCATION)
      set(MAYA_LOCATION "/Applications/Autodesk/")
    endif()
    
    set(MAYA_LOCATION_FULL "${MAYA_LOCATION}/maya${MAYA_VERSION}")
    
    find_path(MAYA_INCLUDE_DIR maya/MFn.h PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES include)
    find_path(MAYA_LIBRARY_DIR libOpenMaya.dylib PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES Maya.app/Contents/MacOS)
elseif(WIN32)
    set(MAYA_PLUGIN_EXT .mll)
    set(MAYA_COMPILE_DEFINITIONS "NT_PLUGIN")
    
    if(NOT DEFINED MAYA_LOCATION)
      set(MAYA_LOCATION "C:/Program Files/Autodesk/")
    endif()
    
    set(MAYA_LOCATION_FULL "${MAYA_LOCATION}/Maya${MAYA_VERSION}")
    
    find_path(MAYA_INCLUDE_DIR maya/MFn.h PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES include)
    find_path(MAYA_LIBRARY_DIR OpenMaya.lib PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES lib)
else()
    set(MAYA_PLUGIN_EXT .so)
    
    if(NOT DEFINED MAYA_LOCATION)
      set(MAYA_LOCATION "/usr/autodesk/")
    endif()
    
    set(MAYA_LOCATION_FULL "${MAYA_LOCATION}/maya${MAYA_VERSION}")
    
    find_path(MAYA_INCLUDE_DIR maya/MFn.h PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES include)
    find_path(MAYA_LIBRARY_DIR libOpenMaya.so PATHS ${MAYA_LOCATION_FULL} PATH_SUFFIXES lib)
endif()

# Find Maya libraries
set(_MAYA_LIBRARIES Foundation OpenMaya OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI)
foreach(MAYA_LIB ${_MAYA_LIBRARIES})
    find_library(MAYA_${MAYA_LIB}_LIBRARY NAMES ${MAYA_LIB} PATHS ${MAYA_LIBRARY_DIR} NO_DEFAULT_PATH)
    if (MAYA_${MAYA_LIB}_LIBRARY)
        set(MAYA_LIBRARIES ${MAYA_LIBRARIES} ${MAYA_${MAYA_LIB}_LIBRARY})
    endif()
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MAYA DEFAULT_MSG MAYA_LOCATION MAYA_INCLUDE_DIR MAYA_LIBRARIES)
