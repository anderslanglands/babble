#include "babble"

#include <memory>

// test smart pointer delegation

class Foo;
using FooPtr = std::unique_ptr<Foo>;
using ConstFooPtr = std::unique_ptr<Foo const>;

class Foo {
    int a = 5;
public:

    static FooPtr create() { return std::unique_ptr<Foo>(new Foo()); }
    int get_foo() const { return a; }
    int set_foo(int a);
};

BBL_MODULE(test027) {
    bbl::Class<Foo>("Foo")
        .m(&Foo::create)
        .m(&Foo::get_foo)
        .m(&Foo::set_foo)
    ;

    bbl::Class<FooPtr>("FooPtr")
        .ctor(bbl::Class<FooPtr>::Ctor<>(), "new")
        .smartptr_to<Foo>()
        ;

    bbl::Class<ConstFooPtr>("ConstFooPtr")
        .ctor(bbl::Class<ConstFooPtr>::Ctor<>(), "new")
        .smartptr_to<const Foo>()
        ;
}