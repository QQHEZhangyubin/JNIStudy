cmake_minimum_required(VERSION 3.22.1)

project(AnkerEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(ConfigureAllModules)  


set(CMAKE_CXX_STANDARD 17)

set(AKSS_CURA_ENGINE cloud)

add_subdirectory(CuraEngine)
add_subdirectory(sliceservice)

#add_subdirectory(demo)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS -pthread)
endif()

#__add_real_target(DemoCXSlice.Com exe SOURCE AnkerEngine.cpp LIB sliceservice DEF SLICE_SERVICE_DLL)
__add_real_target(AnkerEngine dll SOURCE AnkerEngine.cpp LIB sliceservice DEF SLICE_SERVICE_DLL)



