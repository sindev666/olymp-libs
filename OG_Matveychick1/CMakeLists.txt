cmake_minimum_required(VERSION 3.25)
project(OG_Matveychick1)

set(CMAKE_CXX_STANDARD 23)

add_executable(OG_Matveychick1 main.cpp
)
add_executable(OG_Matveychick2 main1.cpp
)
if (OG_Matveychick1)
    add_definitions(-DOG_Matveychick1=1)
endif (OG_Matveychick1)
