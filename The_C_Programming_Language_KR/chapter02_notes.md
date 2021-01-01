# Chapter 2 - Types, operators and expressions

- char:  		1
- short: 		2
- int:   		4
- long:  		8
- float: 		4
- double:		8
- long double:	16

- use `void` as function input parameter if the function doesn't accept parameters
- `static` keyword initializes the variable with 0
- `const` keyword if the variable is unmutable (can be used in function parameter declaration)

sing extension: negative `char` will convert to negative `int` (on some machines)

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
