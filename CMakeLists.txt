cmake_minimum_required(VERSION 3.10)
project(Assignment C)

set(CMAKE_C_STANDARD 99)

add_executable(EggShell main.c CLI.c linenoise.c)