cmake_minimum_required(VERSION 3.27)

project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

file (GLOB test_SRC
    "core/*.h"
    "core/*.hpp"
    "core/*.cpp"
    "core/*.c"
)

add_executable(test ${test_SRC} "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/glad/src/glad.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SDL_IMAGE_LIBRARY "C:/Program Files(x86)/SDL3_Image")
set(SDL_IMAGE_INCLUDE_DIR "C:/Program Files(x86)/SDL3_Image/include")

set(FLECS_LIBRARY "C:/Program Files (x86)/flecs/lib/flecs_static.lib")

set(CGLM_LIBRARY "C:/Users/jal4dsox/lib/cglm/build/cglm.lib")
set(CGLM_INCLUDE_DIR "C:/Users/jal4dsox/lib/cglm/include/cglm")

set(RLGL_INCLUDE_DIR "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/Raylib")

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

target_include_directories(test
    PUBLIC ${SDL3_INCLUDE_DIRS}
    PUBLIC ${SDL3IMAGE_INCLUDE_DIRS}
    PUBLIC ${FLECS_INCLUDE_DIRS}
    PUBLIC ${RLGL_INDLUDE_DIR}
    PRIVATE ${CGLM_INCLUDE_DIR}
)

target_link_libraries(test PUBLIC ${SDL3_LIBRARIES} ${FLECS_LIBRARY} SDL3_image::SDL3_image  PRIVATE ${CGLM_LIBRARY})

include_directories(
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/glad/include"
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/sokol"
    SYSTEM "C:/Program Files (x86)/flecs/include"
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/Raylib"
)

link_directories(
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/glad/include"
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/sokol"
    SYSTEM "C:/Program Files (x86)/flecs/include"
    SYSTEM "C:/Users/jal4dsox/C++Stuff/SokolFlecsTest/include/Raylib"
)
