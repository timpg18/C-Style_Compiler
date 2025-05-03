# C-style Compiler

A **C-style language compiler** written in **C++**, using **Lex/Flex** for lexical analysis and **YACC/Bison** for parsing. The compiler generates **x86-64 Intel syntax assembly**, assembled with **NASM** and linked with **LD**.

---

## 📦 Dependencies

* `g++` (C++ compiler)
* `flex` (lexer generator)
* `bison` (parser generator)
* `nasm` (assembler)
* `ld` (linker)

---

## 🏗️ Project Structure

This compiler was developed as part of the **Compiler Design** course. It is divided into four assignments:

* **Assignment 1**: Lexical Analysis Phase
* **Assignment 2**: Parsing Phase
* **Assignment 3**: Semantic Analysis and Three Address Code (3AC) Generation
* **Assignment 4**: Assembly Code Generation

---

## 🚀 Running the Project

To run the project:

1. Add the test case in the `testing` folder inside **Assignment 4**.
2. Use `./run.sh` to generate the assembly.
3. Use `./run1.sh` to run the generated assembly.

---

## ✨ Features

### ✅ IR-Level Features (Three Address Code - 3AC)

#### Class Syntax Example

```cpp
class ClassName {
public: {
    int a;
    float b;
}
private: {
    double c;
}
};
```

* **Function calling**, including **variable argument functions (ellipses)**
* **Control flow constructs**: `if-else`, **nested if-else**, `for`, `while`, `do-while`, `until` loops
* **Data structures**: `struct`, `union`, `enum`, `typedef`
* **Object-oriented features**: `class` definitions, object creation and usage, **inheritance**, and access modifiers (`public`, `private`, `protected`)
* **Memory management**: **dynamic memory allocation**, **multilevel pointers and arrays**
* **Modifiers**: `static`, `const` keywords
* **Input/Output**: `printf`, `scanf`
* **Pointer operations**
* **Jump controls**: `goto`, `break`, `continue`
* **Arithmetic and logical operators**
* **Recursive function calls**

### ✅ Assembly-Level Features

* **Function calling**
* **Loops**: `for`, `while`, `do-while`, `until`
* **Conditional constructs**: `if-else`, `switch-case`
* **Type conversions** (automatic and explicit)
* **Pointer operations**
* **Input/Output**: `printf`, `scanf`
* **Jump controls**: `goto`, `break`, `continue`

---

## 🧠 Future Work / Known Issues

* Support for **division operations** in assembly
* Handling **global variables** and `static` variables with proper `.data` and `.bss` sections
* Implementation of **advanced arithmetic expressions** with short-circuit logic (`||`, `&&`) integrated into arithmetic
* Support for **assignment to variables referenced through pointers**
* Handling of **member function calls** for `class` objects
* Support for **passing arrays and structs as arguments**
* Support for **returning values beyond integers** (e.g., structs, floats, etc.)
* Support for **implicit type conversions**

---

## 👥 Contributors

* PARIT
* PRATYAKSH
* RONIT
* AVIRAL
