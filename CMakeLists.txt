cmake_minimum_required(VERSION 3.8)
project(Fraktale VERSION 1.0.0
                 DESCRIPTION "Interactive Fractal Explorer"
                 LANGUAGES CXX)

set(APP_TARGET Fraktale)
set(CMAKE_CXX_STANDARD 11)

find_package(CMAKERC REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(IMGUI REQUIRED)
find_package(OpenGL REQUIRED)

set(GLFW_USE_STATIC_LIBS ON)
set(GLEW_USE_STATIC_LIBS ON)

file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${APP_TARGET} MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${APP_TARGET} PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(${APP_TARGET} PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${APP_TARGET} PUBLIC ${GLM_INCLUDE_DIRS})
target_include_directories(${APP_TARGET} PUBLIC ${IMGUI_INCLUDE_DIRS})
target_include_directories(${APP_TARGET} PUBLIC ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${APP_TARGET} PRIVATE glfw)
target_link_libraries(${APP_TARGET} PRIVATE GLEW::GLEW)
target_link_libraries(${APP_TARGET} PRIVATE glm::glm)
target_link_libraries(${APP_TARGET} PRIVATE imgui::imgui)
target_link_libraries(${APP_TARGET} PRIVATE OpenGL::GL)

cmrc_add_resource_library(
    Shaders
    ALIAS Fraktale::Shaders
    NAMESPACE Shaders
    WHENCE shaders
    shaders/VertexShader.vert
    shaders/FragmentShader.frag
)
target_link_libraries(${APP_TARGET} PRIVATE Fraktale::Shaders)