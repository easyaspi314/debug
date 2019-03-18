#include "debug.h"
struct bar {
    int x;
};

int main() {
    struct bar value = { .x = 0 };
    debug(value); // should trigger a static assert on the GCC family
}
