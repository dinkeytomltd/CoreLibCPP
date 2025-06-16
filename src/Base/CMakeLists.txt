
# CoreLib.Base - Essential System types
cmake_minimum_required(VERSION 3.15)

# Collect all Base module source files
file(GLOB_RECURSE BASE_SOURCES
    "*.cpp"
    "Text/*.cpp"
    "Text/RegularExpressions/*.cpp"
    "Coroutines/*.cpp"
    "Runtime/CompilerServices/*.cpp"
    "GC/*.cpp"
)

file(GLOB_RECURSE BASE_HEADERS
    "../../include/System/*.h"
    "../../include/System/Text/*.h"
    "../../include/System/Text/RegularExpressions/*.h"
    "../../include/System/Linq/*.hpp"
    "../../include/System/Coroutines/*.hpp"
    "../../include/System/Runtime/CompilerServices/*.h"
    "../../include/System/GC/*.hpp"
)

# Exclude files that belong to other modules
list(FILTER BASE_HEADERS EXCLUDE REGEX ".*/(Threading|Collections|IO|Reflection)/.*")

add_library(CoreLib.Base STATIC ${BASE_SOURCES})

target_include_directories(CoreLib.Base 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
)

# External dependencies - Boost.Multiprecision (header-only)
# Note: Boost setup is handled in the main CMakeLists.txt via FetchBoost.cmake
if(TARGET Boost::multiprecision)
    target_link_libraries(CoreLib.Base PUBLIC Boost::multiprecision)
    message(STATUS "CoreLib.Base: Linked with Boost::multiprecision")
else()
    message(FATAL_ERROR "CoreLib.Base requires Boost::multiprecision target. Ensure Boost is properly configured.")
endif()

# RTTR dependency for reflection support
find_package(rttr CONFIG QUIET)
if(rttr_FOUND)
    target_link_libraries(CoreLib.Base PUBLIC RTTR::Core)
    target_compile_definitions(CoreLib.Base PUBLIC CORELIB_REFLECTION_AVAILABLE)
    message(STATUS "CoreLib.Base: Linked with RTTR for reflection support")
else()
    message(WARNING "CoreLib.Base: RTTR not found. Delegate reflection features will be disabled.")
endif()

# Boehm GC dependency for garbage collection support
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(BOEHM_GC QUIET bdwgc)
    if(BOEHM_GC_FOUND)
        target_link_libraries(CoreLib.Base PUBLIC ${BOEHM_GC_LIBRARIES})
        target_include_directories(CoreLib.Base PUBLIC ${BOEHM_GC_INCLUDE_DIRS})
        target_compile_definitions(CoreLib.Base PUBLIC CORELIB_GC_BOEHM)
        message(STATUS "CoreLib.Base: Linked with Boehm GC for garbage collection support")
    else()
        # Try to find libgc manually
        find_library(GC_LIBRARY NAMES gc bdwgc)
        find_path(GC_INCLUDE_DIR gc/gc.h)
        if(GC_LIBRARY AND GC_INCLUDE_DIR)
            target_link_libraries(CoreLib.Base PUBLIC ${GC_LIBRARY})
            target_include_directories(CoreLib.Base PUBLIC ${GC_INCLUDE_DIR})
            target_compile_definitions(CoreLib.Base PUBLIC CORELIB_GC_BOEHM)
            message(STATUS "CoreLib.Base: Linked with Boehm GC library at ${GC_LIBRARY}")
        else()
            message(WARNING "CoreLib.Base: Boehm GC not found. Garbage collection will use reference counting only.")
        endif()
    endif()
else()
    # Try to find libgc manually without pkg-config
    find_library(GC_LIBRARY NAMES gc bdwgc)
    find_path(GC_INCLUDE_DIR gc/gc.h)
    if(GC_LIBRARY AND GC_INCLUDE_DIR)
        target_link_libraries(CoreLib.Base PUBLIC ${GC_LIBRARY})
        target_include_directories(CoreLib.Base PUBLIC ${GC_INCLUDE_DIR})
        target_compile_definitions(CoreLib.Base PUBLIC CORELIB_GC_BOEHM)
        message(STATUS "CoreLib.Base: Linked with Boehm GC library at ${GC_LIBRARY}")
    else()
        message(WARNING "CoreLib.Base: Boehm GC not found. Garbage collection will use reference counting only.")
    endif()
endif()

# Set C++20 standard
target_compile_features(CoreLib.Base PUBLIC cxx_std_20)

# Enable coroutine support
if(MSVC)
    target_compile_options(CoreLib.Base PUBLIC /await:strict /std:c++20)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(CoreLib.Base PUBLIC -fcoroutines)
endif()

# Set target properties
set_target_properties(CoreLib.Base PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
