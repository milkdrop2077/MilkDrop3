cmake_minimum_required(VERSION 3.10)

add_library(milkdrop_nseel
    asm-nseel-ppc-gcc.c
    asm-nseel-x86-gcc.c
    asm-nseel-x86-msvc.c
    nseel-caltab.c
    nseel-cfunc.c
    nseel-compiler.c
    nseel-eval.c
    nseel-lextab.c
    nseel-ram.c
    nseel-yylex.c
)

target_include_directories(milkdrop_nseel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
