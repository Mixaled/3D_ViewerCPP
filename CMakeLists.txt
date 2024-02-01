cmake_minimum_required(VERSION 3.15)
project(3d_viewer)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_SHARED OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON OFF CACHE BOOL " " FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/nanogui ext_build/nanogui)
set_property(TARGET nanogui glfw glfw_objects PROPERTY FOLDER "dependencies")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/tinyobjloader ext_build/tinyobjloader)

include_directories(
    src/
    ${NANOGUI_EXTRA_INCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nanogui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tinyobjloader
)

add_executable(3d_viewer
    src/main.cpp
    src/view.cpp
    src/controller.cpp
    src/model.cpp
)

add_definitions(${NANOGUI_EXTRA_DEFS})
target_link_libraries(3d_viewer nanogui ${NANOGUI_EXTRA_LIBS} tinyobjloader)

configure_file(src/assets/cube.obj ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/bash/converter.sh ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/bash/gif_converter.sh ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/config/config.conf ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/shaders/fragment_shader.frag ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/shaders/vertex_shader.vert ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/shaders/points.frag ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/shaders/vertex_point.vert ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

if (MSVC)
  target_compile_options(3d_viewer PRIVATE /W0)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(3d_viewer PRIVATE -w)
endif ()
