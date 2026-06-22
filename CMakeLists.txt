cmake_minimum_required(VERSION 3.16)

project(OpenGLTest)

# C++17を使用
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFWを探す
find_package(glfw3 REQUIRED)

# ヘッダファイル検索パス
include_directories(
    include
)

# 実行ファイル作成
add_executable(OpenGLTest
    main.cpp
    glad/glad.c
)

# macOSのOpenGL Framework
target_link_libraries(OpenGLTest
    glfw
    "-framework OpenGL"
)