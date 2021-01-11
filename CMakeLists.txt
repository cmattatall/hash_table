cmake_minimum_required(VERSION 3.18)

option(BUILD_TESTING "[ON/OFF] Build tests in addition to library." OFF)
option(BUILD_EXAMPLES "[ON/OFF] Build examples in addition to library." OFF)
option(VERBOSE "[ON/OFF] Emit verbose messages during configuration" OFF)

macro(log_verbose msg)
    if(VERBOSE)
        message(${msg} ${ARGN})
    endif(VERBOSE)
endmacro(log_verbose msg)


project(
    C_HASH_TABLE
    VERSION 1.0
    DESCRIPTION "HASH TABLE IMPLEMENTATION IN CASE I EVER NEED IT IN THE FUTURE"
    LANGUAGES C CXX
)
set(CURRENT_TARGET ${PROJECT_NAME})
message("Configuring target : ${PROJECT_NAME}")


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(BUILD_TESTING ON FORCE)
    set(BUILD_EXAMPLES ON FORCE)

    set(${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES "")
    list(APPEND ${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES "Debug")
    list(APPEND ${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES "Release")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES})
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type chosen by the user at configure time" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST ${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES)
        message("Build type : ${CMAKE_BUILD_TYPE} is not a supported build type.")
        message("Supported build types are:")
        foreach(type ${${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES})
            message("- ${type}")
        endforeach(type ${${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES})
        message(FATAL_ERROR "The configuration script will now exit.")
    endif()


    if(WIN32)
        set(CMAKE_EXECUTABLE_SUFFIX ".exe")
    elseif(UNIX AND NOT APPLE)
        set(CMAKE_EXECUTABLE_SUFFIX ".out")
    elseif(APPLE)
        set(CMAKE_EXECUTABLE_SUFFIX ".out")
    else()
        message(FATAL_ERROR "[ERROR] platform: ${CMAKE_HOST_SYSTEM_NAME} not supported")
    endif()

    foreach(btype ${${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES})
        string(TOUPPER ${btype} type)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${type} "${PROJECT_BINARY_DIR}/${btype}/bin" 
            CACHE INTERNAL "Output directory for executables built in \"${btype}\" mode"
        )

        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${type} "${PROJECT_BINARY_DIR}/${btype}/lib" 
            CACHE INTERNAL "Output directory for archived binaries built in \"${btype}\" mode"
        )

        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${type} "${PROJECT_BINARY_DIR}/${btype}/lib" 
            CACHE INTERNAL "Output directory for libraries built in \"${btype}\" mode"
        )
    endforeach(btype ${${CMAKE_PROJECT_NAME}_SUPPORTED_BUILD_TYPES})

    execute_process(
        COMMAND "git" "submodule" "update" "--remote" "--recursive"
        RESULT_VARIABLE GIT_SUBMODULE_UPDATE_FAILED
        OUTPUT_VARIABLE GIT_SUBMODULE_UPDATE_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ECHO_OUTPUT_VARIABLE
    )
    execute_process(COMMAND "sync")
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

if(TARGET ${CURRENT_TARGET})
    get_target_property(type ${CURRENT_TARGET} TYPE)
    if(type STREQUAL STATIC_LIBRARY)

    elseif(type STREQUAL MODULE_LIBRARY)

    elseif(type STREQUAL SHARED_LIBRARY)

    elseif(type STREQUAL OBJECT_LIBRARY)

    elseif(type STREQUAL INTERFACE_LIBRARY)

    elseif(type STREQUAL EXECUTABLE)

    elseif(type STREQUAL UTILITY) # added by add_custom_target

    endif()

    message(FATAL_ERROR "Target ${CURRENT_TARGET} already exists with type ${type}")
else()
    add_library(${CURRENT_TARGET})
endif(TARGET ${CURRENT_TARGET})


# PRIVATE SOURCES
get_property(${CURRENT_TARGET}_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
foreach(LANG ${${CURRENT_TARGET}_languages})
    log_verbose("Indexing ${LANG} sources...")
    foreach(EXTENSION ${CMAKE_${LANG}_SOURCE_FILE_EXTENSIONS})
        set(GLOBBING_EXPR "${CMAKE_CURRENT_SOURCE_DIR}/src/*.${EXTENSION}")
        log_verbose("Indexing ${CURRENT_TARGET} sources that match ${GLOBBING_EXPR}")
        file(GLOB_RECURSE ${LANG}_${EXTENSION}_sources "${GLOBBING_EXPR}")
        list(APPEND ${CURRENT_TARGET}_${LANG}_sources ${${LANG}_${EXTENSION}_sources})
        foreach(src ${${LANG}_${EXTENSION}_sources})
            log_verbose("\t${src}")
        endforeach(src ${${LANG}_${EXTENSION}_sources})
    endforeach(EXTENSION ${CMAKE_${LANG}_SOURCE_FILE_EXTENSIONS})
    list(REMOVE_DUPLICATES ${CURRENT_TARGET}_${LANG}_sources)
    target_sources(${CURRENT_TARGET} PRIVATE ${${CURRENT_TARGET}_${LANG}_sources})
endforeach(LANG ${${CURRENT_TARGET}_languages})


# PRIVATE HEADERS
foreach(LANG ${${CURRENT_TARGET}_languages})
    if(${LANG} STREQUAL CXX)
        file(GLOB_RECURSE ${CURRENT_TARGET}_${LANG}_private_hdrs 
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        )
    elseif(${LANG} STREQUAL C)
        file(GLOB_RECURSE ${CURRENT_TARGET}_${LANG}_private_hdrs 
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        )
    elseif(${LANG} STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${CURRENT_TARGET}_${LANG}_private_inc_dirs "")
    foreach(hdr ${${CURRENT_TARGET}_${LANG}_private_hdrs})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${CURRENT_TARGET}_${LANG}_private_inc_dirs ${hdr_dir})
    endforeach(hdr ${${CURRENT_TARGET}_${LANG}_private_hdrs})
    list(REMOVE_DUPLICATES ${CURRENT_TARGET}_${LANG}_private_inc_dirs)
    target_include_directories(${CURRENT_TARGET} PRIVATE ${${CURRENT_TARGET}_${LANG}_private_inc_dirs})
endforeach(LANG ${${CURRENT_TARGET}_languages})


# PUBLIC HEADERS
foreach(LANG ${${CURRENT_TARGET}_languages})
    if(LANG STREQUAL CXX)
        file(GLOB_RECURSE ${CURRENT_TARGET}_${LANG}_public_hdrs 
            "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp"
        )
    elseif(LANG STREQUAL C)
        file(GLOB_RECURSE ${CURRENT_TARGET}_${LANG}_public_hdrs 
            "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h"
        )
    elseif(LANG STREQUAL ASM)
    
    else()
        # language not supported
    endif()

    set(${CURRENT_TARGET}_${LANG}_public_inc_dirs "")
    foreach(hdr ${${CURRENT_TARGET}_${LANG}_public_hdrs})
        get_filename_component(hdr_dir ${hdr} DIRECTORY)
        list(APPEND ${CURRENT_TARGET}_${LANG}_public_inc_dirs ${hdr_dir})
    endforeach(hdr ${${CURRENT_TARGET}_${LANG}_public_hdrs})
    list(REMOVE_DUPLICATES ${CURRENT_TARGET}_${LANG}_public_inc_dirs)
    target_include_directories(${CURRENT_TARGET} PUBLIC ${${CURRENT_TARGET}_${LANG}_public_inc_dirs})
endforeach(LANG ${${CURRENT_TARGET}_languages})

# CONFIGURE TESTS
if(BUILD_TESTING)
    enable_testing()
    include(CTest)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_subdirectory(tests)
    elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")
        add_subdirectory(test)
    endif()
endif(BUILD_TESTING)

# CONFIGURE EXAMPLES
if(BUILD_EXAMPLES)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples")
        add_subdirectory(examples)
    elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/example")
        add_subdirectory(example)
    endif()
endif(BUILD_EXAMPLES)


################################################################################
#   USER CONFIGURATION STARTS HERE
################################################################################

# EXAMPLE OF LINKINAG AGAINST PTHREAD
#   find_package(Threads REQUIRED)
#   if(Threads_FOUND) 
#       if(WIN32)
#           if(CMAKE_USE_WIN32_THREADS_INIT)
#               target_link_libraries(${CURRENT_TARGET} PUBLIC pthread)
#               target_compile_options(${CURRENT_TARGET} PUBLIC "-lpthread")
#           else()
#               message(FATAL_ERROR "BLAHHH ")
#           endif()
#       elseif(UNIX)
#           if(CMAKE_USE_PTHREADS_INIT)
#               target_link_libraries(${CURRENT_TARGET} PUBLIC pthread)
#               target_compile_options(${CURRENT_TARGET} PUBLIC "-lpthread")
#           else()
#               message(FATAL_ERROR "BLAHHH ")
#           endif()
#       endif()
#   else()
#       message(FATAL_ERROR "No threading libraries have been found. Aborting!")
#   endif()



















