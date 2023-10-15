#include <stdfunction-c.h>

void fn(stdfunction_Foo_t* foo, stdfunction_Foo_t** result) {
    stdfunction_Foo_copy(foo, result);
    stdfunction_Foo_set_a(*result, 42);
}

int main() {
    stdfunction_Bar_adjust_foo(fn);
    return 0; 
}