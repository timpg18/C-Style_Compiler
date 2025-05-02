# C-style Compiler

A **C-style language compiler** written in **C++**, using **Lex/Flex** for lexical analysis and **YACC/Bison** for parsing. The compiler generates **x86-64 Intel syntax assembly**, assembled with **NASM** and linked with **LD**.

## Dependencies
- `g++` (C++ compiler)
- `flex` (lexer generator)
- `bison` (parser generator)
- `nasm` (assembler)
- `ld` (linker)

## Features (Implemented till IR Generation)
- **Function calling**, including **variable argument functions (ellipses)**
- **Control flow constructs**: `if-else`, **nested if-else**, `for`, `while`, `do-while`, `until` loops
- **Data structures**: `struct`, `union`, `enum`, `typedef`
- **Object-oriented features**: `class` definitions, object creation and usage
- **Memory management**: **dynamic memory allocation**, **multilevel pointers and arrays**
- **Modifiers**: `static`, `const` keywords
- **Input/Output**: `printf`, `scanf`
- **Pointer operations**
- **Jump controls**: `goto`, `break`, `continue`
- **Arithmetic and logical operators**
- **Recursive function calls**

Intermediate Representation (IR) is generated as **Three Address Code (3AC)**.

## Features (Implemented till Assembly Generation)
- **Function calling**
- **Loops**: `for`, `while`, `do-while`, `until`
- **Conditional constructs**: `if-else`, `switch-case`
- **Type conversions** (automatic and explicit)
- **Pointer operations**
- **Input/Output**: `printf`, `scanf`
- **Jump controls**: `goto`, `break`, `continue`
- **structs, enums, union**
- **printf, dynamic memory allocation**

