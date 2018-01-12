if(APPLE)
    # Set default plugin install directory
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ~/Library/Preferences/Autodesk/maya/${MAYA_VERSION}/plug-ins)
    
    # This assumes that Maya is installed to the cannonical location and that the devkit is also extracted there
    set(MAYA_PLUGIN_EXT .bundle)
    set(MAYA_COMPILE_DEFINITIONS "OSMac_;MAC_PLUGIN")
    set(MAYA_LOCATION /Applications/Autodesk/maya${MAYA_VERSION})
    
    find_path(MAYA_INCLUDE_DIR maya/MFn.h PATHS ${MAYA_LOCATION} PATH_SUFFIXES devkit/include)
    find_path(MAYA_LIBRARY_DIR libOpenMaya.dylib PATHS ${MAYA_LOCATION} PATH_SUFFIXES Maya.app/Contents/MacOS)
    
    # Find Maya libraries
    set(_MAYA_LIBRARIES Foundation OpenMaya OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI)
    foreach(MAYA_LIB ${_MAYA_LIBRARIES})
        find_library(MAYA_${MAYA_LIB}_LIBRARY NAMES ${MAYA_LIB} PATHS ${MAYA_LIBRARY_DIR} NO_DEFAULT_PATH)
        if (MAYA_${MAYA_LIB}_LIBRARY)
            set(MAYA_LIBRARIES ${MAYA_LIBRARIES} ${MAYA_${MAYA_LIB}_LIBRARY})
        endif()
    endforeach()
else()
    #TODO: setup windows and linux builds
    message(SEND_ERROR Windows and linux builds are currently not supported!)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MAYA DEFAULT_MSG MAYA_LOCATION MAYA_INCLUDE_DIR MAYA_LIBRARIES)
