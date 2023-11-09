#include "babble"

#include <memory>

// test smart pointer delegation

class Foo;
using FooPtr = std::unique_ptr<Foo>;
using ConstFooPtr = std::unique_ptr<Foo const>;

class Bar;
using BarPtr = std::unique_ptr<Bar>;
using ConstBarPtr = std::unique_ptr<Bar const>;

class Foo {
    int a = 5;
public:

    static FooPtr create() { return FooPtr(new Foo()); }
    int get_foo() const { return a; }
    int set_foo(int a);
};

class Bar : public Foo {
public:
    static BarPtr create() { return BarPtr(new Bar()); }
    void baz();
};

BBL_MODULE(test031) {
    bbl::Class<Foo>("Foo")
        .m(&Foo::create)
        .m(&Foo::get_foo)
        .m(&Foo::set_foo)
    ;

    bbl::Class<FooPtr>("FooPtr")
        .smartptr_to<Foo>()
        ;

    bbl::Class<ConstFooPtr>("ConstFooPtr")
        .smartptr_to<const Foo>()
        ;

    bbl::Class<Bar>()
        .inherits_from<Foo>()
        .m(&Bar::create)
        .m(&Bar::baz)
        ;

    bbl::Class<BarPtr>("BarPtr")
        .smartptr_to<Bar>()
        ;

    bbl::Class<ConstBarPtr>("ConstBarPtr")
        .smartptr_to<const Bar>()
        ;

}