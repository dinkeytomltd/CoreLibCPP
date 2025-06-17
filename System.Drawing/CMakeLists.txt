cmake_minimum_required(VERSION 3.15)
project(SystemDrawing VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(SYSTEM_DRAWING_BUILD_TESTS "Build System.Drawing tests" ON)
option(SYSTEM_DRAWING_BUILD_EXAMPLES "Build System.Drawing examples" ON)
option(SYSTEM_DRAWING_USE_SYSTEM_SKIA "Use system-installed Skia" OFF)

# DirectX backend support (Windows only)
if(WIN32)
    option(SYSTEM_DRAWING_ENABLE_DIRECTX "Enable DirectX backend for System.Drawing" ON)
    option(SYSTEM_DRAWING_ENABLE_GDIPLUS "Enable GDI+ backend for System.Drawing" ON)
else()
    set(SYSTEM_DRAWING_ENABLE_DIRECTX OFF)
    set(SYSTEM_DRAWING_ENABLE_GDIPLUS OFF)
endif()

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
elseif(UNIX)
    set(PLATFORM_LINUX TRUE)
endif()

# Compiler-specific settings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
    if(PLATFORM_WINDOWS)
        # Use clang-cl on Windows for better optimization
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -O2")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2")
endif()

# Find or build Skia
if(SYSTEM_DRAWING_USE_SYSTEM_SKIA)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SKIA REQUIRED skia)
else()
    # Build Skia from source
    include(ExternalProject)
    
    set(SKIA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/skia)
    set(SKIA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/skia-build)
    set(SKIA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/skia-install)
    
    # Skia build arguments for minimal static build
    set(SKIA_GN_ARGS
        "is_official_build=true"
        "is_component_build=false"
        "skia_use_system_freetype2=false"
        "skia_use_system_harfbuzz=false"
        "skia_use_system_icu=false"
        "skia_use_system_libjpeg_turbo=false"
        "skia_use_system_libpng=false"
        "skia_use_system_libwebp=false"
        "skia_use_system_zlib=false"
        "skia_enable_gpu=true"
        "skia_enable_pdf=false"
        "skia_enable_svg=false"
        "skia_enable_skottie=false"
        "skia_enable_tools=false"
        "skia_enable_ccpr=false"
        "skia_enable_nvpr=false"
    )
    
    # Platform-specific Skia configuration
    if(PLATFORM_WINDOWS)
        list(APPEND SKIA_GN_ARGS "clang_win=\"C:/Program Files/LLVM\"")
        list(APPEND SKIA_GN_ARGS "target_cpu=\"x64\"")
    elseif(PLATFORM_MACOS)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            list(APPEND SKIA_GN_ARGS "target_cpu=\"arm64\"")
        else()
            list(APPEND SKIA_GN_ARGS "target_cpu=\"x64\"")
        endif()
    elseif(PLATFORM_LINUX)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            list(APPEND SKIA_GN_ARGS "target_cpu=\"arm64\"")
        else()
            list(APPEND SKIA_GN_ARGS "target_cpu=\"x64\"")
        endif()
    endif()
    
    # Convert list to string
    string(REPLACE ";" " " SKIA_GN_ARGS_STR "${SKIA_GN_ARGS}")
    
    ExternalProject_Add(
        skia_external
        SOURCE_DIR ${SKIA_SOURCE_DIR}
        BINARY_DIR ${SKIA_BUILD_DIR}
        CONFIGURE_COMMAND 
            ${CMAKE_COMMAND} -E chdir ${SKIA_SOURCE_DIR}
            python3 tools/git-sync-deps
        COMMAND
            ${CMAKE_COMMAND} -E chdir ${SKIA_SOURCE_DIR}
            bin/gn gen ${SKIA_BUILD_DIR} --args='${SKIA_GN_ARGS_STR}'
        BUILD_COMMAND
            ${CMAKE_COMMAND} -E chdir ${SKIA_BUILD_DIR}
            ninja skia
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS ${SKIA_BUILD_DIR}/libskia.a
    )
    
    # Create imported target for Skia
    add_library(skia STATIC IMPORTED)
    set_target_properties(skia PROPERTIES
        IMPORTED_LOCATION ${SKIA_BUILD_DIR}/libskia.a
        INTERFACE_INCLUDE_DIRECTORIES ${SKIA_SOURCE_DIR}
    )
    add_dependencies(skia skia_external)
endif()

# DirectX detection and configuration
if(SYSTEM_DRAWING_ENABLE_DIRECTX AND WIN32)
    # Find DirectX components
    find_path(DirectX_INCLUDE_DIR
        NAMES d2d1.h
        PATHS
            "${CMAKE_WINDOWS_KITS_10_DIR}/Include/*/um"
            "${CMAKE_WINDOWS_KITS_10_DIR}/Include/*/shared"
            "C:/Program Files (x86)/Windows Kits/10/Include/*/um"
            "C:/Program Files (x86)/Windows Kits/10/Include/*/shared"
            "C:/Program Files/Windows Kits/10/Include/*/um"
            "C:/Program Files/Windows Kits/10/Include/*/shared"
    )
    
    find_library(DirectX_D2D1_LIBRARY
        NAMES d2d1
        PATHS
            "${CMAKE_WINDOWS_KITS_10_DIR}/Lib/*/um/x64"
            "C:/Program Files (x86)/Windows Kits/10/Lib/*/um/x64"
            "C:/Program Files/Windows Kits/10/Lib/*/um/x64"
    )
    
    find_library(DirectX_DWRITE_LIBRARY
        NAMES dwrite
        PATHS
            "${CMAKE_WINDOWS_KITS_10_DIR}/Lib/*/um/x64"
            "C:/Program Files (x86)/Windows Kits/10/Lib/*/um/x64"
            "C:/Program Files/Windows Kits/10/Lib/*/um/x64"
    )
    
    if(DirectX_INCLUDE_DIR AND DirectX_D2D1_LIBRARY AND DirectX_DWRITE_LIBRARY)
        set(DirectX_FOUND TRUE)
        set(DirectX_LIBRARIES ${DirectX_D2D1_LIBRARY} ${DirectX_DWRITE_LIBRARY} windowscodecs)
        set(DirectX_INCLUDE_DIRS ${DirectX_INCLUDE_DIR})
        message(STATUS "DirectX backend enabled for System.Drawing")
        add_compile_definitions(SYSTEM_DRAWING_DIRECTX_ENABLED)
    else()
        message(WARNING "DirectX not found, DirectX backend disabled")
        set(SYSTEM_DRAWING_ENABLE_DIRECTX OFF)
    endif()
endif()

# GDI+ detection and configuration (Windows only)
if(SYSTEM_DRAWING_ENABLE_GDIPLUS AND WIN32)
    # GDI+ is built into Windows, just need to link
    message(STATUS "GDI+ backend enabled for System.Drawing")
    add_compile_definitions(SYSTEM_DRAWING_GDIPLUS_ENABLED)
    
    # GDI+ libraries
    set(GDIPLUS_LIBRARIES gdiplus)
endif()

# System.Drawing library
add_library(SystemDrawing STATIC
    # Core classes
    src/Graphics.cpp
    src/Image.cpp
    src/Bitmap.cpp
    src/Color.cpp
    src/Point.cpp
    src/Size.cpp
    src/Rectangle.cpp
    src/Brush.cpp
    src/Pen.cpp
    src/Font.cpp
    src/FontFamily.cpp
    
    # Backend infrastructure
    src/GraphicsConfiguration.cpp
    src/GraphicsBackendFactory.cpp
    
    # Drawing2D namespace
    src/Drawing2D/Matrix.cpp
    src/Drawing2D/GraphicsPath.cpp
    src/Drawing2D/LinearGradientBrush.cpp
    
    # Imaging namespace
    src/Imaging/ImageFormat.cpp
    src/Imaging/ImageCodecInfo.cpp
    
    # Skia backend implementations
    src/SkiaBackend/SkiaGraphics.cpp
    src/SkiaBackend/SkiaImage.cpp
    src/SkiaBackend/SkiaFont.cpp
    src/SkiaBackend/SkiaBackend.cpp
)

# Add DirectX backend source files if enabled
if(SYSTEM_DRAWING_ENABLE_DIRECTX AND DirectX_FOUND)
    target_sources(SystemDrawing PRIVATE
        src/DirectXBackend/DirectXBackend.cpp
    )
endif()

# Add GDI+ backend source files if enabled
if(SYSTEM_DRAWING_ENABLE_GDIPLUS AND WIN32)
    target_sources(SystemDrawing PRIVATE
        src/GdiPlusBackend/GdiPlusBackend.cpp
        src/GdiPlusBackend/GdiPlusImage.cpp
        src/GdiPlusBackend/GdiPlusBitmap.cpp
        src/GdiPlusBackend/GdiPlusFont.cpp
    )
endif()

# Include directories
target_include_directories(SystemDrawing
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
if(SYSTEM_DRAWING_USE_SYSTEM_SKIA)
    target_link_libraries(SystemDrawing PUBLIC ${SKIA_LIBRARIES})
    target_include_directories(SystemDrawing PUBLIC ${SKIA_INCLUDE_DIRS})
    target_compile_options(SystemDrawing PUBLIC ${SKIA_CFLAGS_OTHER})
else()
    target_link_libraries(SystemDrawing PUBLIC skia)
    target_include_directories(SystemDrawing PRIVATE 
        ${SKIA_SOURCE_DIR}
        ${SKIA_SOURCE_DIR}/include/core
        ${SKIA_SOURCE_DIR}/include/effects
        ${SKIA_SOURCE_DIR}/include/gpu
        ${SKIA_SOURCE_DIR}/include/utils
    )
endif()

# Platform-specific libraries
if(PLATFORM_WINDOWS)
    target_link_libraries(SystemDrawing PUBLIC
        gdi32
        user32
        kernel32
        ole32
        oleaut32
        uuid
        advapi32
        shell32
        comdlg32
        winspool
    )
    
    # Add DirectX libraries if enabled
    if(SYSTEM_DRAWING_ENABLE_DIRECTX AND DirectX_FOUND)
        target_include_directories(SystemDrawing PRIVATE ${DirectX_INCLUDE_DIRS})
        target_link_libraries(SystemDrawing PRIVATE ${DirectX_LIBRARIES})
    endif()
    
    # Add GDI+ libraries if enabled
    if(SYSTEM_DRAWING_ENABLE_GDIPLUS)
        target_link_libraries(SystemDrawing PRIVATE ${GDIPLUS_LIBRARIES})
    endif()
elseif(PLATFORM_MACOS)
    find_library(CORE_FOUNDATION CoreFoundation)
    find_library(CORE_GRAPHICS CoreGraphics)
    find_library(CORE_TEXT CoreText)
    find_library(CORE_SERVICES CoreServices)
    target_link_libraries(SystemDrawing PUBLIC
        ${CORE_FOUNDATION}
        ${CORE_GRAPHICS}
        ${CORE_TEXT}
        ${CORE_SERVICES}
    )
elseif(PLATFORM_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    target_link_libraries(SystemDrawing PUBLIC
        ${FONTCONFIG_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        pthread
        dl
    )
    target_include_directories(SystemDrawing PUBLIC
        ${FONTCONFIG_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
    )
endif()

# Compiler definitions
target_compile_definitions(SystemDrawing
    PUBLIC
        SYSTEM_DRAWING_EXPORTS
    PRIVATE
        SK_GANESH=1
        SK_GL=1
)

# Tests
if(SYSTEM_DRAWING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Backend summary
message(STATUS "System.Drawing Backend Support:")
message(STATUS "  Skia:    ${SYSTEM_DRAWING_USE_SYSTEM_SKIA}")
if(WIN32)
    message(STATUS "  DirectX: ${SYSTEM_DRAWING_ENABLE_DIRECTX}")
    message(STATUS "  GDI+:    ${SYSTEM_DRAWING_ENABLE_GDIPLUS}")
endif()

# Examples
if(SYSTEM_DRAWING_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS SystemDrawing
    EXPORT SystemDrawingTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT SystemDrawingTargets
    FILE SystemDrawingTargets.cmake
    NAMESPACE SystemDrawing::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SystemDrawing
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SystemDrawingConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/SystemDrawingConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SystemDrawingConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SystemDrawing
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SystemDrawingConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SystemDrawingConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SystemDrawing
)
