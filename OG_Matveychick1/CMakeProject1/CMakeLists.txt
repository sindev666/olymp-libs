cmake_minimum_required(VERSION 3.26)
project(OG_Matveychick1)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
)

add_executable(Codeforces Codeforces.cpp
)

add_executable(another another.cpp grader.h
)

add_executable(Yandex Yandex.cpp
)

add_executable(checker checker.cpp
)

add_executable(gen gen.cpp
)

add_executable(stress stress.cpp
)

add_executable(wrong wrong.cpp
)

add_executable(correct correct.cpp
)

add_executable(A A.cpp
)

add_executable(B B.cpp
)

add_executable(C C.cpp
)

add_executable(D D.cpp
)

add_executable(grader grader.cpp grader.h B.cpp
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=nehalem")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -msse4.2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")

add_definitions(-DOG_Matveychick1)
#add_definitions(-O3)

MATH(EXPR stack_size "256 * 1024 * 1024") # 256 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")