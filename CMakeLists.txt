cmake_minimum_required(VERSION 3.6)
project(minecraft_clone)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp AABB.cpp Block.cpp BlockAir.cpp BlockDirt.cpp BlockGlowstoneLamp.cpp BlockGrass.cpp Blocks.cpp BlockStone.cpp BlockWater.cpp BoundingBox.cpp Camera.cpp Chunk.cpp DebugDrawer.cpp GraphicsManager.cpp Perlin.cpp Player.cpp simplexnoise.cpp vec.cpp World.cpp PhysicsObj.cpp PhysicsObj.h Box.cpp Box.h)

include_directories(~/Documents/Programming/ext/glm)
include_directories(~/Documents/Programming/ext/glew/include)
include_directories(~/Documents/Programming/ext/SFML/include)

link_directories(~/Documents/Programming/ext/glew/lib)
link_directories(~/Documents/Programming/ext/SFML/lib)

add_executable(minecraft_clone ${SOURCE_FILES})

find_package(OpenGL)

target_link_libraries(minecraft_clone ${OPENGL_LIBRARIES})
target_link_libraries(minecraft_clone GLEW.a)
target_link_libraries(minecraft_clone sfml-system.a)
target_link_libraries(minecraft_clone sfml-window.a)
target_link_libraries(minecraft_clone sfml-graphics.a)