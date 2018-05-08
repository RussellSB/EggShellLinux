cmake_minimum_required(VERSION 3.10)
project(Assignment C)

set(CMAKE_C_STANDARD 99)

add_executable(EggShell eggshell.c linenoise.c cmdController.c vrblController.c intrnlCmdController.c extrnlCmdController.c ioController.c)