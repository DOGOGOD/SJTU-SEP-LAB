cmake_minimum_required(VERSION 3.30)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wimplicit-fallthrough -Wswitch -Werror)
if(WIN32)
    add_link_options(-static)
endif()

add_executable(y64sim
    instructions.cpp
    instructions.h
    main.cpp
    memory.cpp
    memory.h
    registers.cpp
    registers.h
    simulator.cpp
    simulator.h
    simulator_utils.cpp
)
