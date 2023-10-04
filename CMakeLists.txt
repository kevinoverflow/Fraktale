cmake_minimum_required(VERSION 3.8)
project(Fraktale)

set(CMAKE_CXX_STANDARD 11)
set(APP_TARGET Fraktale)
set(APP_VERSION 1.0.0)

find_package(CMAKERC REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(IMGUI REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLFW3_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${IMGUI_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

cmrc_add_resource_library(
    Shaders
    ALIAS Fraktale::Shaders
    NAMESPACE Shaders
    WHENCE shaders
    shaders/VertexShader.vert
    shaders/FragmentShader.frag
)

add_executable(${APP_TARGET} MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${APP_TARGET} PRIVATE
    ${GLFW3_LIBRARIES}
    GLEW::GLEW
    glm::glm
    imgui::imgui
    ${OPENGL_LIBRARIES} 
    Fraktale::Shaders
    )