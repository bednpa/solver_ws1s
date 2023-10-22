cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "g++")
project(ws1s_solver)


add_library(ast STATIC src/ast/ast.cpp src/ast/ast.h)
target_include_directories(ast PUBLIC include)

add_library(astdump STATIC src/ast/astdump.cpp)
target_include_directories(astdump PUBLIC include)

add_library(freevars STATIC src/ast/freevars.cpp)
target_include_directories(freevars PUBLIC include)

add_library(ident STATIC src/ast/ident.cpp src/ast/ident.h)
target_include_directories(ident PUBLIC include)

add_library(offsets STATIC src/ast/offsets.cpp src/ast/offsets.h)
target_include_directories(offsets PUBLIC include)

add_library(parser STATIC src/ast/parser.cpp src/ast/parser.hpp)
target_include_directories(parser PUBLIC include)

add_library(predlib STATIC src/ast/predlib.cpp src/ast/predlib.h)
target_include_directories(predlib PUBLIC include)

add_library(printline STATIC src/ast/printline.cpp src/ast/printline.h)
target_include_directories(printline PUBLIC include)

add_library(scanner STATIC src/ast/scanner.cpp)
target_include_directories(scanner PUBLIC include)

add_library(symboltable STATIC src/ast/symboltable.cpp src/ast/symboltable.h)
target_link_libraries(symboltable offsets printline astdump ident)
target_include_directories(symboltable PUBLIC include)

add_library(untyped STATIC src/ast/untyped.cpp src/ast/untyped.h)
target_link_libraries(untyped ast)
target_include_directories(untyped PUBLIC include)

add_library(atomaut STATIC src/core/atomaut.cpp src/core/atomaut.h)
target_include_directories(atomaut PUBLIC include)

add_library(auxiliary STATIC src/core/auxiliary.cpp src/core/auxiliary.h)
target_link_libraries(auxiliary atomaut)
target_include_directories(auxiliary PUBLIC include)

add_library(eval STATIC src/core/eval.cpp src/core/eval.h)
target_include_directories(eval PUBLIC include)

add_library(gencode STATIC src/core/gencode.cpp)
target_link_libraries(gencode ast freevars)
target_include_directories(gencode PUBLIC include)

add_library(unused STATIC src/core/unused.cpp src/core/unused.h)
target_include_directories(unused PUBLIC include)

add_library(explicit_lts_sim STATIC src/mata/explicit_lts_sim.cc src/mata/nfa.hh)
target_include_directories(explicit_lts_sim PUBLIC include)

add_library(inter-aut STATIC src/mata/inter-aut.cc src/mata/nfa.hh)
target_include_directories(inter-aut PUBLIC include)

add_library(nfa-complement STATIC src/mata/nfa-complement.cc src/mata/nfa.hh)
target_include_directories(nfa-complement PUBLIC include)

add_library(nfa-concatenation STATIC src/mata/nfa-concatenation.cc src/mata/nfa.hh)
target_include_directories(nfa-concatenation PUBLIC include)

add_library(nfa-intersection STATIC src/mata/nfa-intersection.cc src/mata/nfa.hh)
target_include_directories(nfa-intersection PUBLIC include)

add_library(nfa STATIC src/mata/nfa.cc src/mata/nfa.hh)
target_link_libraries(nfa explicit_lts_sim inter-aut)
target_include_directories(nfa PUBLIC include)

add_library(parser_mata STATIC src/mata/parser.cc src/mata/parser.hh)
target_include_directories(parser_mata PUBLIC include)

add_executable(ws1s_solver src/main.cpp)
target_link_libraries(ws1s_solver ast astdump freevars ident offsets parser predlib printline scanner symboltable untyped atomaut auxiliary eval gencode unused explicit_lts_sim inter-aut nfa-complement nfa-concatenation nfa-intersection nfa parser_mata)
target_include_directories(ws1s_solver PUBLIC include)

set_target_properties(ws1s_solver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
