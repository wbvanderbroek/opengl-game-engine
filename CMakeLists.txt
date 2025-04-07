cmake_minimum_required(VERSION 3.10)
project(opengl-game-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/Engine/Libraries/include
    ${CMAKE_SOURCE_DIR}/Engine/imgui
    "C:/Program Files/Mono/include/mono-2.0"
)

link_directories(${CMAKE_SOURCE_DIR}/Engine/Libraries/lib)
link_directories("C:/Program Files/Mono/lib")

file(GLOB_RECURSE ENGINE_SOURCES "Engine/source/Engine/*.cpp")
file(GLOB IMGUI_SOURCES "Engine/imgui/*.cpp")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/Engine/Libraries/include/glad/glad.c")

add_executable(opengl-game-engine
    Engine/Main.cpp
    ${ENGINE_SOURCES}
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

target_link_libraries(opengl-game-engine
    glfw3
    assimp-vc143-mt
    mono-2.0-sgen
)

add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Engine/Assets
        $<TARGET_FILE_DIR:opengl-game-engine>/Assets
)

if(WIN32)
    add_custom_command(TARGET opengl-game-engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Engine/Libraries/dll/assimp-vc143-mt.dll"
            "$<TARGET_FILE_DIR:opengl-game-engine>"
    )
endif()
