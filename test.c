#include "debug.h"

int answer(void) {
    return 42;
}

int main(void) {

    int num = 1;
    const char *str = "hello";
    int x = 'X';
    char c = 'X';

    // works:
    debug_raw("These should print letters:");
    debug_raw("Find the", 'X');
    // => Find the X
    debug_raw("Find the", (char)'X');
    // => Find the X
    debug_raw("Find the", (char)x);
    // => Find the X
    debug_raw("Find the", 'X' + 1);
    // => Find the Y
    debug_raw("Find the", 1 + 'X');
    // => Find the Y
    debug_raw("Find the", c);
    // => Find the X
    debug_raw("Find the", "X"[0]);
    // => Find the X
    debug_raw("Find the", (char)('X'));
    // => Find the X
    debug_raw("Find the", (char)88);
    // => Find the X
    debug_raw("Find the", (char)(1 + 'X' + 1));
    // => Find the Z
    debug_raw("Find the", x + '\0');
    // => Find the X

    // doesn't work:
    debug_raw("These should print integers:");
    // parentheses disable the check
    debug_raw("Find the", ('X'));
    // => Find the 88
    // doesn't start or end in a single quote
    debug_raw("Find the", 1 + 'X' + 1);
    // => Find the 90
    // it is a variable, no way to tell without cast
    debug_raw("Find the", x);
    // => Find the 88
    // This only works with char and int:
    debug_raw("Find the", 1u + 'X');
    // => Find the 88
    debug_raw("Find the", 1ll + 'X');
    // => Find the 88


    debug(num);
    // => num = 1

    debug(num, str, answer());
    // => num = 1
    //    str = hello
    //    answer() = 42

    debug_raw("counting:", 1, -2, 3 + 0.4);
    // => counting: 1 -2 3.4

    debug_raw(str, "world!");
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
