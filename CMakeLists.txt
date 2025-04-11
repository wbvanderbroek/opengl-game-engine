cmake_minimum_required(VERSION 3.10)
project(opengl-game-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MONO_DIR "${CMAKE_SOURCE_DIR}/Engine/Mono")

include_directories(
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/Engine/Libraries/include
    ${CMAKE_SOURCE_DIR}/Engine/imgui
    "${MONO_DIR}/include/mono-2.0"
)

link_directories(${CMAKE_SOURCE_DIR}/Engine/Libraries/lib)
link_directories("${MONO_DIR}/lib")

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

set(OUTPUT_DIR $<TARGET_FILE_DIR:opengl-game-engine>)
set(ENGINE_DIR ${OUTPUT_DIR}/Engine)
set(GAME_DIR ${OUTPUT_DIR}/Game)

add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ENGINE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GAME_DIR}

    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:opengl-game-engine> ${ENGINE_DIR}/opengl-game-engine.exe
    COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:opengl-game-engine>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Engine/Libraries/dll/assimp-vc143-mt.dll"
        ${ENGINE_DIR}

        COMMAND ${CMAKE_COMMAND} -E make_directory ${ENGINE_DIR}/Mono/lib/mono

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${MONO_DIR}/bin
            ${ENGINE_DIR}/Mono/bin
        
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${MONO_DIR}/etc
            ${ENGINE_DIR}/Mono/etc

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${MONO_DIR}/lib/mono/4.5
            ${ENGINE_DIR}/Mono/lib/mono/4.5
        

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MONO_DIR}/bin/mono-2.0-sgen.dll"
        ${ENGINE_DIR}
    

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Engine/CoreAssets
        ${ENGINE_DIR}/CoreAssets

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Game/Assets
        ${GAME_DIR}/Assets

    COMMAND ${CMAKE_COMMAND} -E echo @echo off > ${OUTPUT_DIR}/game.bat
    COMMAND ${CMAKE_COMMAND} -E echo cd /d %%~dp0Engine >> ${OUTPUT_DIR}/game.bat
    COMMAND ${CMAKE_COMMAND} -E echo start \"\" \"opengl-game-engine.exe\" >> ${OUTPUT_DIR}/game.bat

    COMMAND ${CMAKE_COMMAND} -E echo @echo off > ${OUTPUT_DIR}/editor.bat
    COMMAND ${CMAKE_COMMAND} -E echo cd /d %%~dp0Engine >> ${OUTPUT_DIR}/editor.bat
    COMMAND ${CMAKE_COMMAND} -E echo start \"\" \"opengl-game-engine.exe\" --editor >> ${OUTPUT_DIR}/editor.bat
)
