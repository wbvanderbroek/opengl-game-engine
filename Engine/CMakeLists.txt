file(GLOB ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_library(Engine ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Libraries/include
    ${PROJECT_SOURCE_DIR}/Game/include
)

target_link_libraries(Engine PUBLIC
    glad
    glfw3
    Game
)
