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

if(APPLE)
    find_package(glfw3 REQUIRED)
    
elseif(WIN32)
    add_subdirectory(external/glfw/glfw-3.4)
endif()

add_executable(OpenGLTest
    main.cpp
    Shader.cpp
    external/glad/src/glad.c
    stb_image.h
)

target_include_directories(OpenGLTest PRIVATE
    external/glfw/glfw-3.4/include
    external/glad/include
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
