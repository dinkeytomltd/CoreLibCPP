
# Examples configuration
add_executable(BasicDrawingExample
    basic_drawing.cpp
)

target_link_libraries(BasicDrawingExample
    PRIVATE
        SystemDrawing
)

target_include_directories(BasicDrawingExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Backend comparison example
add_executable(BackendComparisonExample
    backend_comparison.cpp
)

target_link_libraries(BackendComparisonExample
    PRIVATE
        SystemDrawing
)

target_include_directories(BackendComparisonExample
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# DirectX-specific example (Windows only)
if(SYSTEM_DRAWING_ENABLE_DIRECTX AND DirectX_FOUND)
    add_executable(DirectXSpecificExample
        directx_specific.cpp
    )
    
    target_link_libraries(DirectXSpecificExample
        PRIVATE
            SystemDrawing
    )
    
    target_include_directories(DirectXSpecificExample
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
endif()
