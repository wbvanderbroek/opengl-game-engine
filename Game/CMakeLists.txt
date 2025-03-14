file(GLOB GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/Game/*.cpp")

add_library(Game ${GAME_SOURCES})

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Engine/include
)
