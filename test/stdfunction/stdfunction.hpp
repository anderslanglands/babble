#pragma once

#include <cstdio>
#include <functional>
namespace tst {

class Foo {
    int _a = 0;
public:

    void set_a(int a) {
        _a = a;
    }

    int get_a() const {
        return _a;
    }
};

using FooFn = std::function<Foo (Foo)>;

class Bar {
public:
    static void adjust_foo(FooFn fn) {
        Foo foo_before;
        Foo foo = fn(foo_before);
        printf("foo is now %d\n", foo.get_a());
    }
};

}