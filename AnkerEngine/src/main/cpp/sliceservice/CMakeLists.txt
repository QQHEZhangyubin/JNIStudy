set(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/AnkerCloudSlice.h
        ${CMAKE_CURRENT_SOURCE_DIR}/AnkerCloudSlice.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sliceservice.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sliceservice.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ParamUtil.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ParamUtil.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ProcessHelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ProcessHelper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/commondef.h
        ${CMAKE_CURRENT_SOURCE_DIR}/typedef.h)


set(libs _CuraEngine)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/../CuraEngine/src)
set(DEFS SLICE_SERVICE_DLL)

message("ANDROID_ABI: ${ANDROID_ABI}")
message("qqqqqqq0, $ENV{USR_INSTALL_ROOT}")
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    message("qqqqqqq0-1, $ENV{USR_INSTALL_ROOT}")
    include_directories("$ENV{USR_INSTALL_ROOT}/include/boost-1_78")
else ()
    message("qqqqqqq0-2, $ENV{USR_INSTALL_ROOT}")
    #    include_directories("$ENV{USR_INSTALL_ROOT}/include/boost")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
    #    link_directories("$ENV{USR_INSTALL_ROOT}/lib")
    #    link_directories("/Users/anker/AndroidStudioProjects/JNIStudy/AnkerEngine/src/main/cpp/sliceservice/thirdlib/lib")
    #    link_libraries(boost_filesystem boost_locale boost_thread)


    #    set(libboost_filesystem ${CMAKE_CURRENT_SOURCE_DIR}/thirdlib/lib/libboost_filesystem_1_78.a)
    #    add_library(libboost_filesystem-lib STATIC IMPORTED)
    #    set_target_properties(libboost_filesystem-lib PROPERTIES IMPORTED_LOCATION  ${libboost_filesystem})


    set(libboost_locale "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libboost_locale_1_78.a")
    add_library(libboost_locale-lib STATIC IMPORTED)
    set_target_properties(libboost_locale-lib PROPERTIES IMPORTED_LOCATION ${libboost_locale})

    set(libboost_thread "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libboost_thread_1_78.a")
    add_library(libboost_thread-lib STATIC IMPORTED)
    set_target_properties(libboost_thread-lib PROPERTIES IMPORTED_LOCATION ${libboost_thread})

    set(libboost_filesystem "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libboost_filesystem_1_78.a")
    add_library(libboost_filesystem-lib STATIC IMPORTED)
    set_target_properties(libboost_filesystem-lib PROPERTIES IMPORTED_LOCATION ${libboost_filesystem})


    #-----------------------------------------


    add_library(libiconv SHARED IMPORTED)

    set_target_properties(libiconv
            PROPERTIES IMPORTED_LOCATION
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libiconv.so")


    add_library(libicuuc STATIC IMPORTED)

    set_target_properties(libicuuc PROPERTIES IMPORTED_LOCATION
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libicuuc.a"
            )

    target_link_libraries(
            libboost_locale-lib INTERFACE
            libicuuc
    )

    target_link_libraries(
            libboost_locale-lib INTERFACE
            c++_shared
            libiconv
    )


    add_library(libicui18n STATIC IMPORTED)

    set_target_properties(libicui18n PROPERTIES IMPORTED_LOCATION
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libicui18n.a"
            )

    target_link_libraries(
            libboost_locale-lib INTERFACE
            libicui18n
    )

    add_library(libicudata STATIC IMPORTED)

    set_target_properties(libicudata PROPERTIES IMPORTED_LOCATION
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libicudata.a"
            )

    target_link_libraries(
            libboost_locale-lib INTERFACE
            libicudata
    )

    #-----------------------------------------


    #    link_libraries(libboost_locale-lib libboost_thread-lib libboost_filesystem-lib)
    link_libraries(libboost_filesystem-lib libboost_thread-lib  libboost_locale-lib)


    #    link_libraries("$ENV{USR_INSTALL_ROOT}/boost/1.82.0_1/lib/boost_filesystem"
    #            "$ENV{USR_INSTALL_ROOT}/boost/1.82.0_1/lib/boost_locale"
    #            "$ENV{USR_INSTALL_ROOT}/boost/1.82.0_1/lib/boost_thread")

    set(CMAKE_CXX_FLAGS -pthread)
endif ()

#link_directories("$ENV{USR_INSTALL_ROOT}/lib")
message("$ENV{USR_INSTALL_ROOT}")

link_libraries(_CuraEngine)

option(ENABLE_OPENMP "Enable OPENMP multithreading acceleration" ON)

if (ENABLE_OPENMP)
    FIND_PACKAGE(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        message("OPENMP FOUND ${OpenMP_C_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif ()
endif ()

__add_real_target(sliceservice dll SOURCE ${SRCS} INC ${INCS} LIB ${LIBS} DEF ${DEFS}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} OPENMP)

