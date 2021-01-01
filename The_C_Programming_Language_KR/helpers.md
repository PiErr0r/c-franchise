# Helpers for programming in C
// based on  O'Reilly - Practical C Programming

## Debuggin in situ

```c
#include <stdio.h>
#define DEBUG
#undef DEBUG /* to remove the debugs */

int debug = 1; /* set to 0 to remobe the debug */

int main(void) {
  printf("Always printed!\n");

  if (debug)
    printf("Printed if debug is 1.\n");

/* makes the compiled code bigger and creates 2 versions but is faster */
#ifdef DEBUG
  printf("Printed if DEBUG is defined.\n");
#endif

  return 0;
}
```


## Relative cost of operations

| Operation | Relative cost |
| --- | --- |
| `printf` and `scanf` | 1000 |
| `malloc` and `free` | 800 |
| trig fncs (`sin`, `cos` ...) | 500 |
| `float` (any operation) | 100 |
| `int` divide | 30 |
| `int` multiple | 20 |
| fnc call | 10 |
| array index | 6 |
| shifts | 5 |
| add/subtract | 5 |
| pointer dereference | 2 |
| bitwise and, or, not | 1 |
| logical and, ot, not | 1 |

- **Macros**: Using a macro eliminates the overhead associated with a function call. It also makes the code bigger and a little more difficult to debug.
- **Pointers**: Using pointers is faster than indexing an array. Pointers are, however, more tricky to use.
- **Powers of 2**: Use a power of 2 when doing integer multiply or divide. Most compilers substitute a shift for the operation.

## Makefile 

The Makefile contains the following sections:
- Comments
- Macros
- Explicit rules
- Default rules

### Comments 

every line which begins with #

### Macros

A macro has the format:
`name = data`
where name is any valid identifier and data is the text that will be substituted whenever make sees $(name).

### Explicit rules

Explicit rules tell make what commands are needed to create the program. They can take several forms. The most common of these is:
```make
target: source [source2] [source3]
        command
        [command]
        [command]
        ...
```
where target is the name of a file to create

Another form of the explicit rule is:
```make
source: command
        [command]
```

`hist.o: ia.h hist.c` tells make to create hist.o from hist.c and ia.h , using the standard suffix rule for making file.o from file.c. 

This rule is:
`$(CC) $(CFLAGS) -c file.c`
(make predefines the macros $(CC) and $(CFLAGS).)


```make
CFLAGS= -g
CC=cc
OBJ=ia.o hist.o

all: hist

hist: $(OBJ)
  $(CC) $(CFLAGS) -o hist $(OBJ)
hist.o:ia.h hist.c
ia.o:ia.h ia.c
```

The macro OBJ is a list of all the object ( .o) files. The lines:
```make
hist: $(OBJ) $(CC) $(CFLAGS) -o hist $(OBJ)
```
tell make to create hist from the ob ject files. If any of the object files are out of date, make will re-create them.

The line: `hist.o:hist.c ia.h` tells make to create `hist.o` from `ia.h` and `hist.c`.

```make
#-----------------------------------------------#
# Makefile for UNIX systems                     #
# using a GNU C compiler.                       #
#-----------------------------------------------#
CC=cc
CFLAGS=-g
#
# Compiler flags:
# -g    -- Enable debugging
ia: ia.c
    $(CC) $(CFLAGS) -o ia ia.c
clean:
    rm -f ia
```
