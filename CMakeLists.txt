cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)
project(ImWidgets LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    message(STATUS "Configuring for Windows")

    # Add ImGui and your own headers
    include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/imgui/backends
        "C:/Users/manto/Documents/dev/pkgs/glfw-3.4/include"
    )

    # Link against prebuilt GLFW static libs
    link_directories(
        "C:/Users/manto/Documents/dev/pkgs/glfw-3.4/lib-mingw-w64"
    )

    # Static link options for MinGW
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
    add_definitions(-DGLFW_STATIC)

    set(PLATFORM_LIBS
        glfw3
        opengl32
        gdi32
        user32
    )

else()  # Linux
    message(STATUS "Configuring for Linux")

    # Find GLFW using pkg-config
    find_package(OpenGL REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)

    # Include paths
    include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/imgui/backends
        ${GLFW_INCLUDE_DIRS}
    )

    link_directories(${GLFW_LIBRARY_DIRS})

    set(PLATFORM_LIBS
        ${GLFW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        dl
        pthread
    )
endif()

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(ImWidgets
    src/main.cpp
    src/im_widgets.cpp
    ${IMGUI_SOURCES}
)

target_link_libraries(ImWidgets ${PLATFORM_LIBS})
