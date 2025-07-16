cmake_minimum_required(VERSION 3.20)
project(hill LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cc
)

add_executable(picasoSingh ${SOURCES})


target_include_directories(picasoSingh PRIVATE include)

target_link_libraries(picasoSingh PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

