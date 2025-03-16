cmake_minimum_required(VERSION 3.10)
project(opengl-game-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libraries)
add_subdirectory(Engine)
add_subdirectory(Game)

add_executable(opengl-game-engine Main.cpp)

if(APPLE)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(opengl-game-engine PRIVATE Engine glfw OpenGL::GL assimp)
elseif(LINUX)
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(opengl-game-engine PRIVATE Engine glfw OpenGL::GL assimp)
elseif(WIN32)
    target_link_libraries(opengl-game-engine PRIVATE Engine assimp)
endif()

add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:opengl-game-engine>/Assets
)
add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Libraries/dll/assimp-vc143-mt.dll"
        "$<TARGET_FILE_DIR:opengl-game-engine>"
)
