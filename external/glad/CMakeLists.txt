cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

PROJECT(Glad)

add_library(glad STATIC include/glad/glad.h src/glad.c)
target_include_directories(glad PUBLIC include/)