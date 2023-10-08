#include "babble"

#include <memory>

// test smart pointer delegation

class Foo;
using FooPtr = std::unique_ptr<Foo>;

class Foo {
    int a = 5;
public:

    static FooPtr create() { return std::unique_ptr<Foo>(new Foo()); }
    int get_foo() const { return a; }
};

BBL_MODULE(test027) {
    bbl::Class<Foo>("Foo")
        .m(&Foo::create)
        .m(&Foo::get_foo)
    ;

    bbl::Class<FooPtr>("FooPtr")
        .ctor(bbl::Ctor<FooPtr>(), "new")
        .smartptr_to<Foo>()
        ;
}