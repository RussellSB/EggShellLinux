cmake_minimum_required(VERSION 3.10)
project(Assignment C)

set(CMAKE_C_STANDARD 99)

add_executable(EggShell eggshell.c cmdController.c linenoise.c vrblController.c)