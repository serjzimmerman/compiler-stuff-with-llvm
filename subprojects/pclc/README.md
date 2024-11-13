<!--toc:start-->

- [0. What is ParaCL](#0-what-is-paracl)
  - [Example program in ParaCL](#example-program-in-paracl)
  - [More complex example](#more-complex-example)
  - [Functions](#functions)
- [1. ParaCL Compiler (pclc)](#1-paracl-compiler-pclc)

<!--toc:end-->

Interpreter implementation for ParaCL, a custom C-like language.

## 0. What is ParaCL

ParaCL is a C-like language for teaching about compiler frontend development.

### Example program in ParaCL

Variables are defined on the left-hand side of the assignment (which can be chained with already defined variables). Type of the newly defined variable is deduced from the right-hand side (void can't appear on the right side of assignment for obvious reasons).

```
n = ?;
fact = 1;

while (n > 0)
{
  fact = fact * n;
  n = n - 1;
}

print fact;
```

### More complex example

In ParaCL assignments are chainable and multiple variables can be declared in a single line. Furthermore, like in C, assignment is a statement as well as an expression. Here is a more complex examples which prints the absolute value of the input number in a loop:

```
// Testing assignments as expressions
while ((x = ?) != 0) {
  if ((y = ?) > 0) {
    print y;
  } else {
    print -y;
  }
}
```

As a bonus, single line C++ style comments are allowed.

### Functions

ParaCL supports functions and function pointer variables which can be reassigned. Return types are deduced. By default all argument types are assumed to be `int`:
Here's a recursive fibonacci function:

```
func(x) : rfib1 {
  res = 0;

  if (x == 0) return 0;
  if (x == 1) return 1;

  res = rfib(x - 1) + rfib(x - 2);
  return res;
}
```

This is equivalent:

```
fib = func(int x) : rfib2 {
  int res = 0;
  if (x <= 1) return x;
  res = rfib2(x - 1) + rfib2(x - 2);
  return res;
}
```

Here `fib` is a function pointer which can be reassigned or passed into other functions:

```
print fib(10); // prints 55

func(int func(int) f, int x) : fib_printer {
  return f(x);
}

print fib_printer(fib, 10); // prints 55
```

## 1. ParaCL Compiler (pclc)

The _pclc_ binary is the brain of the whole interpreter. It compiles the source file into an executable for the ParaCL VM and executes it. It's possible to skip the execution and dump the disassembled binary or write it to file.
There is a standalone VM executable _pclvm_ which is used to run the bytecode file.

```sh

pclc --help
# Allowed options:
#  -h, --help                   Print this help message
#  -a, --ast-dump               Dump AST
#  -i, --input [=arg(=)]        Specify input file
#  -o, --output [=arg(=a.out)]  Specify output file for compiled program
#  -d, --disas                  Disassemble generated code (does not run the program)

# Example usage:
pclc examples/scan.pcl

# Or alternatively: 
pclc examples/fib_simple.pcl -o
pclvm a.out

# To dump the disassembled code:
pclc examples/read.pcl -d
# .constant_pool
# 0x00000000 = { 0 }
#
# .code
# 0x00000000 push_const [ 0x00000000 ]
# 0x00000005 push_read
# 0x00000006 mov_local_rel [ 0x00000000 ]
# 0x0000000b push_local_rel [ 0x00000000 ]
# 0x00000010 print
# 0x00000011 pop
# 0x00000012 ret

# Or:
pcldis a.out
```

It is also possible to view the parse tree before semantic analysis. To do this you have to provide -a flag:

```sh
pclc examples/fib_simple.pcl -a > fib.dump
dot -Tpng fib.dump > fib.png
```
