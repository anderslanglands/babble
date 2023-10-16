#include <smartptr-c.h>
#include <stdio.h>

int main(int argc, char** argv) {

    bar_FooPtr_t* foo = nullptr;
    bar_Foo_create(&foo);

    int a = 0;
    bar_FooPtr_get_foo(foo, &a);
    printf("foo is %d\n", a);

    bar_FooPtr_dtor(foo);

    return 0;
}