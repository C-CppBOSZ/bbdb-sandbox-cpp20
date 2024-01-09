cmake_minimum_required(VERSION 3.28)
project(mainBSDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(mainBSDB main.cpp
        bodb.cpp
        bodb.h
        type_provider.cpp
        type_provider.h
        scheme_b.cpp
        scheme_b.h
        src_provider.cpp
        src_provider.h)
