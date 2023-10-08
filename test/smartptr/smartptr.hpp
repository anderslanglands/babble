#pragma once

#include <stdio.h>
#include <memory>

class Foo;
using FooPtr = std::unique_ptr<Foo>;

class Foo {
    int a = 5;
public:

    static FooPtr create() { return std::unique_ptr<Foo>(new Foo()); }
    int get_foo() const { return a; }
};