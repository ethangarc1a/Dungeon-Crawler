cmake_minimum_required(VERSION 3.10)
project(TacticalDungeonCrawler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dungeon_crawler main.cpp)

# Enable warnings
if(MSVC)
    target_compile_options(dungeon_crawler PRIVATE /W4)
else()
    target_compile_options(dungeon_crawler PRIVATE -Wall -Wextra -pedantic)
endif()
