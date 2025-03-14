cmake_minimum_required(VERSION 3.10)
project(opengl-game-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libraries)
add_subdirectory(Engine)
add_subdirectory(Game)

add_executable(opengl-game-engine Main.cpp)
target_link_libraries(opengl-game-engine PRIVATE Engine)

add_custom_command(TARGET opengl-game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:opengl-game-engine>/Assets
)