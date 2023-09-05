# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project(CuraEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ConfigureAllModules)


if(APPLE)
    set(OPENMP_ROOT "/opt/homebrew/Cellar/libomp/14.0.6/")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")
    if (M1)
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
        set(OPENMP_ROOT "/Users/anker/mac_x64_86/libomp/14.0.0/")
    endif()
endif()



option (ENABLE_VLD    "Enable VLD to check memory" OFF)
option (ENABLE_OPENMP "Enable OPENMP multithreading acceleration" ON)
option (TEST_CL "Static library test engine" OFF)


if(TEST_CL AND WIN32)
    add_definitions(-DANKER_DEBUG)
endif()

if(ENABLE_VLD)
    __enable_vld()
endif ()
if(ENABLE_OPENMP)
    message("ENABLE_OPENMP......")
    __enable_openmp()
endif ()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/clipper)
set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/rapidjson)
set(EIGEN_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/eigen)
set(STB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb)
set(BOOST_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/libs/boost)
set(BOOST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/boost)

add_subdirectory(src)


set(LIBS _CuraEngine)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/src)
message("ENGINE: jjjjjjjjj")
if (WIN32)
    set(LIBS _CuraEngine DbgHelp.lib)
endif()
message("ENGINE: jjjjjjjjj1")
__add_real_target(CuraEngine dll SOURCE main.cpp dbgcrash.cpp LIB ${LIBS} INC ${INCS} OPENMP)
message("ENGINE: jjjjjjjjj2")


if(TEST_CL AND WIN32)
    include(TestLibCL)
    __test_lib_cl()
endif()


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.



#add_library( # Sets the name of the library.
#        AnkerEngine
#
#        # Sets the library as a shared library.
#        SHARED
#
#        # Provides a relative path to your source file(s).
#        AnkerEngine.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#find_library( # Sets the name of the path variable.
#        log-lib
#
#        # Specifies the name of the NDK library that
#        # you want CMake to locate.
#        log)
#
## Specifies libraries CMake should link to your target library. You
## can link multiple libraries, such as libraries you define in this
## build script, prebuilt third-party libraries, or system libraries.
#
#target_link_libraries( # Specifies the target library.
#        AnkerEngine
#
#        # Links the target library to the log library
#        # included in the NDK.
#        ${log-lib})

#target_link_libraries(AnkerEngine CuraEngine)