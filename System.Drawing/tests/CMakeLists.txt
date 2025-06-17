
# Test configuration
find_package(GTest QUIET)

if(GTest_FOUND)
    # Create test executable
    add_executable(SystemDrawingTests
        test_color.cpp
        test_point.cpp
        test_size.cpp
        test_rectangle.cpp
        test_graphics.cpp
        test_backend_selection.cpp
        test_directx_backend.cpp
    )
    
    target_link_libraries(SystemDrawingTests
        PRIVATE
            SystemDrawing
            GTest::gtest
            GTest::gtest_main
    )
    
    target_include_directories(SystemDrawingTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # Add tests to CTest
    include(GoogleTest)
    gtest_discover_tests(SystemDrawingTests)
else()
    message(STATUS "Google Test not found, skipping tests")
endif()
