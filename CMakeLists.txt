cmake_minimum_required(VERSION 3.10)
project(opengl-game-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/Libraries/include
    ${CMAKE_SOURCE_DIR}/imgui
)

link_directories(${CMAKE_SOURCE_DIR}/Libraries/lib)

file(GLOB_RECURSE ENGINE_SOURCES "source/Engine/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "source/Game/*.cpp")
file(GLOB IMGUI_SOURCES "imgui/*.cpp")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/Libraries/include/glad/glad.c")

add_executable(opengl-game-engine
    Main.cpp
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

target_link_libraries(opengl-game-engine
    glfw3
    assimp-vc143-mt
)

add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:opengl-game-engine>/Assets
)

if(WIN32)
    add_custom_command(TARGET opengl-game-engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Libraries/dll/assimp-vc143-mt.dll"
            "$<TARGET_FILE_DIR:opengl-game-engine>"
    )
endif()
