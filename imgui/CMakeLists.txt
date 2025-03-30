file(GLOB IMGUI_SOURCES
    *.cpp
    *.h
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/Libraries/include
)

target_link_libraries(imgui PRIVATE glfw3)
