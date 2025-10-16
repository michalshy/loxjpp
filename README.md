# Lox Interpreter (C++17)

A tree-walk interpreter for the **Lox** programming language, implemented in modern **C++17**, following the structure and design from Robert Nystrom’s *[Crafting Interpreters](https://craftinginterpreters.com/)*.

This project recreates the **jLox** interpreter from the book using C++17 features like smart pointers, RAII, and variants for a clean and maintainable design.

---

## 🚀 Features

- 🧠 **Scanner (Lexer)** — Tokenizes Lox source code.
- 🧩 **Recursive Descent Parser** — Converts tokens into an AST.
- ⚙️ **Interpreter** — Evaluates the AST in a tree-walk manner.
- 💬 **REPL** — Interactive Read-Eval-Print Loop.
- 📄 **Script Execution** — Run `.lox` files directly.
- 🧵 **Error Handling** — Reports syntax and runtime errors clearly.
- 🧱 **C++17** — Modern, safe, and efficient implementation.
- 🧰 **AST Generator Tool** — Automatically generates expression and statement classes.

---

## 🗂️ Project Structure

```
loxcpp/
├── res/
│   ├── cake.lox
│   ├── class_base.lox
│   ├── cook.lox
│   ├── crunch.lox
│   ├── ex1.lox
│   ├── ex2.lox
│   ├── ex3.lox
│   ├── fib.lox
│   ├── fun.lox
│   ├── inheritance.lox
│   └── init.lox
├── src/
│   ├── utils/
│   │   ├── errors.hpp
│   │   ├── object.h
│   │   └── tokens.hpp
│   ├── environment.cpp
│   ├── environment.h
│   ├── expr.h
│   ├── interpreter.cpp
│   ├── interpreter.h
│   ├── lox_callable.h
│   ├── lox_class.cpp
│   ├── lox_class.h
│   ├── lox_function.cpp
│   ├── lox_function.h
│   ├── lox_instance.cpp
│   ├── lox_instance.h
│   ├── lox.cpp
│   ├── lox.h
│   ├── main.cpp
│   ├── parser.h
│   ├── resolver.cpp
│   ├── resolver.h
│   ├── scanner.cpp
│   ├── scanner.h
│   └── stmt.h
├── tools/
│   └── generate_ast/
│       └── main.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

---

## ⚡ Getting Started

### Prerequisites

- **C++17** compatible compiler (GCC ≥ 9.0, Clang ≥ 9.0, or MSVC ≥ 2019)
- **CMake** ≥ 3.15
- **Make** or equivalent build tool

### Build Instructions

```bash
git clone https://github.com/michalshy/loxjpp.git
cd loxcpp
mkdir build && cd build
cmake ..
make
```

This builds the interpreter executable:

```bash
./loxjpp
```

---

## 🧩 Usage

### 🖥️ REPL Mode

Run the interpreter without arguments:

```bash
./loxjpp
```

Example:
```text
> print 1 + 2 * 3;
7
> fun greet(name) { print "Hello, " + name + "!"; }
> greet("world");
Hello, world!
```

### 📜 Script Mode

Execute a Lox script from the `res/` directory:

```bash
./loxjpp res/fib.lox
```

---

## 🧠 Example Code

**res/fib.lox**
```lox
fun fib(n) {
  if (n <= 1) return n;
  return fib(n - 1) + fib(n - 2);
}

for (var i = 0; i < 10; i = i + 1) {
  print fib(i);
}
```

**res/fun.lox**
```lox
fun greet(name) {
  print "Hello, " + name + "!";
}

greet("Lox");
```

## 🛠️ Tools

The `tools/generate_ast/` directory contains a small C++ program that generates the `Expr` and `Stmt` class hierarchies automatically, mirroring the `GenerateAst` tool from *Crafting Interpreters*. Expr header has to be included in Stmt file after.

Build it separately:

```bash
cd tools/generate_ast
mkdir build && cd build
cmake ..
make
./generate_ast
```

---

## 📚 References

- 📘 *Crafting Interpreters* — [craftinginterpreters.com](https://craftinginterpreters.com/)
- 🧾 Lox Grammar — [Appendix I: Grammar](https://craftinginterpreters.com/appendix-i.html)
- 🧠 C++17 Reference — [cppreference.com](https://en.cppreference.com/w/cpp/17)

---

## 💬 Acknowledgments

Huge thanks to **Robert Nystrom** for *Crafting Interpreters*, which inspired this project and countless language enthusiasts.

> “The best way to understand how a language works is to build one.”
> — *Robert Nystrom*

## ! Drawbacks

This project is far from being perfect, was used for following the tutorial.
Usage of some C++ features might be missinterpreted, but I wanted to mirror the source code
as much as possible.