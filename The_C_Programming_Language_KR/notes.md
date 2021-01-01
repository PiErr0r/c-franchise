# Notes
## Chapter 2 - Types, operators and expressions

- use `void` as function input parameter if the function doesn't accept parameters
- `static` keyword initializes the variable with 0 (int) and it is visible only in the file where it is defined (same with the functions declared as static)
- `external` keyword for using some global variable (initialized with 0)
- `const` keyword if the variable is unmutable (can be used in function parameter declaration)
- `register` keyword is for variables which will be heavily used so the compiler will store them in register for faster r/w

sign extension: negative `char` will convert to negative `int` (on some machines)

`x = x & ~077;` is better for anullating the last 6 bits because it is size undependent unlike `x = x & 0177700` which expect x to be 16 bits long

fuck this is some wild stuff

```c
/* gets n bits from position p of integer x
 * e.g. get_bits(26, 4, 3) will return bits 4 3 and 2 => 110 (2)
 * 26 (10) = 011010 (2)
 *           543210
 */
unsigned get_bits(int x, int p, int n) {
  return (x >> (p+1-n) & ~(~0 << n));
}
```

`x &= (x-1);` deletes the rightmost 1-bit of x

```c
enum months { JAN = 1, FEB, MAR, APR, MAY, JUN,
              JUL, AUG, SEP, OCT, NOV, DEC };
/* FEB is 2, MAR is 3, etc. */
```

## Chapter 4 - Functions and program structure

example of debug macro:

- `#define debug(expr) printf(#expr " = %g\n", expr)` => this will replace #expr with text provided to macro
- `##` operator will concatenate arguments passed to macro 
    - `#define paste(front, back) front ## back` => so paste (name, 1) creates the token name1

## Chapter 5 - Pointers and arrays

- `++*p` will increase value of the address pointed by p by 1 but `*p++` will increase the pointer p by 1 and return value stored in new address 
    - because unary operators are evaluated from right to left
    - correct way to postfix increase the value in address pointed by p is `(*p)++`
    - `*w++ = c` will place variable c in `*w` adn then increment pointer w by one
- `int a[5] = {1};` will initialize only the first element as 1, others will be 0

Command-line arguments:
- main is called with 2 parameters `argc` which is argument count and `argv` which is vector of arguments
    - `argc` is at least 1 since `argv[0]` is th ename of the program executed
    - `(*++argv)[0]` will walk through argument strings and take their first char
    - `*++argv[0]` will walk through the chars of the string (argument) `argv` is currently pointing to

## Chapter 6 - Structures

- in binary search using pointers there are some changes:
    - we can't use `mid = (low + high) / 2` because addition of pointers is not allowed
    - but substraction is so we can use `mid = low + (high - low) / 2`
    - and while you can't dereference `&word[n]` you can still use it in arithmetic for calculating mid

Simple hashing (hashmap) algorithm:
```c
/* hash: form hash value for string s */
unsigned hash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s 1= '\0'; s++)
    hashval = *s + 31 * hashval;
  return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s) {
  struct nlist *np;
  for (np = hashtab[hash(s)]; np 1= NULL; np = np->next)
    if (strcmp(s, np->name) == 0)
      return np; 1* found *1
  return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn) {
  struct nlist *np;
  unsigned hashval;
  if ((np = lookup(name)) == NULL) { /* not found */
    np = (struct nlist *) malloc(sizeof(*np));
    if (np == NULL || (np->name = strdup(name)) == NULL)
      return NULL;

    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;

  } else    /* already there */
    free((void *) np->defn); /* free previous defn */

  if ((np->defn = strdup(defn)) == NULL)
    return NULL;

  return np;
}
```

### Union

`union` stores multiple variables with different types in the same memory ([source](https://www.tutorialspoint.com/cprogramming/c_unions.htm))

```c
#include <stdio.h>
#include <string.h>
 
union Data {
   int i;
   float f;
   char str[20];
};
 
int main( ) {

   union Data data;        

   data.i = 10;
   data.f = 220.5;
   strcpy( data.str, "C Programming");

   printf( "data.i : %d\n", data.i);
   printf( "data.f : %f\n", data.f);
   printf( "data.str : %s\n", data.str);

   return 0;
}
```

will print:

```
data.i : 1917853763
data.f : 4122360580327794860452759994368.000000
data.str : C Programming
```

but 

```c
#include <stdio.h>
#include <string.h>
 
union Data {
   int i;
   float f;
   char str[20];
};
 
int main( ) {

   union Data data;        

   data.i = 10;
   printf( "data.i : %d\n", data.i);
   
   data.f = 220.5;
   printf( "data.f : %f\n", data.f);
   
   strcpy( data.str, "C Programming");
   printf( "data.str : %s\n", data.str);

   return 0;
}
```

prints out:

```
data.i : 10
data.f : 220.500000
data.str : C Programming
```

### Bit fields

Definition of a struct which acts as a small int because sizes (in bits) are defined for its members (after colon)

```c
struct {
  unsigned int is_keyword :1;
  unsigned int is_extern  :1;
  unsigned int is static  :1;
} flags;
```


