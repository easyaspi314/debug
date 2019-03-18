# Debugging like a sir (in C)

![](http://oi59.tinypic.com/5yw50z.jpg)

```c
#include "debug.h"

int answer(void) {
    return 42;
}

int main(void) {

    int num = 1;
    const char *str = "hello";

    debug(num);
    // => num = 1

    debug(num, str, answer());
    // => num = 1
    //    str = hello
    //    answer() = 42

    debug_raw("counting:", 1, -2, 3 + 0.4);
    // => counting: 1 -2 3.4

    debug_raw(str, "world", '!');
    // => hello world!

    idebug(num);
    // => example.c:26: num = 1

    idebug(num, str);
    // => example.c:29:
    //            num = 1
    //            str = hello

    idebug_raw("The answer is", answer());
    // => example.c:34: The answer is 42

    return 0;
}
```

## Usage

Just include `debug.h` in your file.

## Gotchas

* Requires C11 support (uses `_Generic`). Consider using the flag `-std=c11` when compiling. Known compilers to work are GCC 4.9+, Clang 3.1+, icc 16+, and tcc 0.9.27 (_Complex isn't supported though).
* Literal chars have type `int`. The only way they can be detected as so is if they are casted to `char`, or if they __lexically__ start or end in a single quote (the arguments are stringified using the preprocessor). If you want to print a char literal as an int, just wrap it in parentheses.

This is the code that checks for it:
```c
#define __is_char_literal(x) (sizeof(#x) >= 4 && ((#x)[0] == '\'' || (#x)[sizeof(#x) - 2] == '\''))

// doing this...
__is_char_literal('x');
// ...expands to basically this code:
const char stringified[] = "'x'";
size_t len = strlen(stringified);

return len >= 3 && (stringified[0] == '\'' || stringified[len - 1] == '\'');
```

It's not perfect, but it actually does a very good job at picking up most cases:
```c
int x = 'X';
char c = 'X';

// works:
debug_raw("Find the", 'X'); // => Find the X
debug_raw("Find the", (char)'X'); // => Find the X
debug_raw("Find the", (char)x); // => Find the X
debug_raw("Find the", 'X' + 1); // => Find the Y
debug_raw("Find the", 1 + 'X'); // => Find the Y
debug_raw("Find the", c); // => Find the X
debug_raw("Find the", "X"[0]); // => Find the X
debug_raw("Find the", (char)('X')); // => Find the X
debug_raw("Find the", (char)88); // => Find the X
debug_raw("Find the", (char)(1 + 'X' + 1)); // => Find the Z
debug_raw("Find the", x + '\0'); // => Find the X

// doesn't work:
// parentheses disable the check
debug_raw("Find the", ('X')); // => Find the 88
// doesn't start or end in a single quote
debug_raw("Find the", 1 + 'X' + 1); // => Find the 90
// it is a variable, no way to tell without cast
debug_raw("Find the", x); // => Find the 88
// This only works with char and int:
debug_raw("Find the", 1u + 'X'); // => Find the 88
debug_raw("Find the", 1ll + 'X'); // => Find the 88
```

## Tips

For quick debugging, include the header right where you need it:

```c
void function(void) {
    ...
    #include "debug.h"
    debug(var); // => var = XXX
    ...
}
```

To remove quickly all debugging output, define `NDEBUG` before the header inclusion:

```c
#define NDEBUG
#include "debug.h"
...
debug(var); // Doesn't print anything
...
```

