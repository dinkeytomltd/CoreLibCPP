
cmake_minimum_required(VERSION 3.10)
project(CoreLibCPP VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find required packages
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/System/Threading/AsyncLocal.cpp
    src/System/Threading/ThreadLocal.cpp
    src/System/Threading/Barrier.cpp
    src/System/Threading/CountdownEvent.cpp
    src/System/Threading/ReaderWriterLockSlim.cpp
    src/System/Threading/Task.cpp
    src/System/Threading/TaskCompletionSource.cpp
    src/System/Threading/CancellationToken.cpp
)

# Create library
add_library(CoreLibCPP STATIC ${SOURCES})

# Link libraries
target_link_libraries(CoreLibCPP Threads::Threads)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(CoreLibCPP PRIVATE -Wall -Wextra -O2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(CoreLibCPP PRIVATE /W4 /O2)
endif()

# Install targets
install(TARGETS CoreLibCPP
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

# Examples (optional)
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests (optional)
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
