cmake_minimum_required(VERSION 3.18)
project(POS_Semestralka)

set(CMAKE_CXX_STANDARD 17)

add_executable(POS_Semestralka main.cpp client.cpp server.cpp client.cpp server.cpp
        client.h)

target_link_libraries(POS_Semestralka pthread)