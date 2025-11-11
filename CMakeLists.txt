cmake_minimum_required(VERSION 3.10.0)
project(Mini_Factory VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Mini_Factory
               main.cpp
               src/Util.cpp
               src/Animation.cpp
               src/AnimationFrames.cpp
               src/Tile.cpp
               src/AnimatedTile.cpp
               src/Pathfinding.cpp
               src/Belt.cpp
               src/ItemPath.cpp)

target_include_directories(Mini_Factory PRIVATE include)

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)

target_link_options(Mini_Factory PRIVATE -mconsole)
target_link_libraries(Mini_Factory SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)
