cmake_minimum_required(VERSION 3.16)

if(WIN32)
    project(OpenGLTest LANGUAGES C CXX)
elseif(APPLE)
    project(OpenGLTest)
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

if(APPLE)
    find_package(glfw3 REQUIRED)
    
elseif(WIN32)
    add_subdirectory(external/glfw/glfw-3.4)
endif()

add_executable(OpenGLTest
    main.cpp
    Shader.cpp
    Camera.cpp
    external/glad/src/glad.c
    stb_image.h
    Objects.cpp
    Shapes/Plane.cpp
    Shapes/Cube.cpp
    Entity/Transform.cpp
    Entity/Object.cpp
    Entity/Player.cpp
    Core/Input.cpp
    global.cpp
    Game.cpp
    Collision/CollisionMesh.cpp
    Collision/Triangle.h
    Collision/RayCast.cpp
    BlockWrold.cpp
    Entity/PlayerManager.cpp
    StageEditor.h
    StageEditor.cpp
    Texture.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    Entity/Goal.cpp
    Collision/AABB.cpp
)
target_include_directories(OpenGLTest PRIVATE
    external/glfw/glfw-3.4/include
    external/glad/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
if(APPLE)
    target_link_libraries(OpenGLTest PRIVATE
        glfw
        "-framework OpenGL"
    )
elseif(WIN32)
    target_link_libraries(OpenGLTest PRIVATE
        glfw
        opengl32
    )
endif()
