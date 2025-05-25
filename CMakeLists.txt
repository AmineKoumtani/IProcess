cmake_minimum_required(VERSION 3.10)
project(IPProcess C)

set(CMAKE_C_STANDARD 99)

add_executable(IPProcess
        main.c
        bmp8.c
        bmp8.h
        bmp24.c
        bmp24.h
        file_utils.c
        file_utils.h
        bmp_const.h
)