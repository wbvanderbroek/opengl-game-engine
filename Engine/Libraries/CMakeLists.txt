add_library(glad STATIC
    include/glad/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
target_link_libraries(glad PUBLIC OpenGL::GL)

add_library(glfw3 INTERFACE)
target_include_directories(glfw3 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(glfw3 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib
)

add_library(assimp INTERFACE)

target_include_directories(assimp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(assimp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc143-mt.lib
)