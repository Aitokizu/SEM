cmake_minimum_required(VERSION 3.24)
project(SEM C)

set(CMAKE_C_STANDARD 11)

add_executable(SEM main.c stud_info.h book_info.h)
