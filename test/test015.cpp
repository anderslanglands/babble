#include "babble"


namespace qux {
class Bar {
public:
    Bar();
    Bar(Bar const& other);
    ~Bar();

    int* baz(char const** s, float *const f) const;
};

class Foo {
public:
    Foo();
    Foo(Foo&& other);
    ~Foo();

    Foo const& foo(Bar& b, int*& i) const;
    void foo2(Bar b) noexcept;
    static int foo3(Bar const& b) noexcept;
};
}

// Test C type conversion zoo
BBL_MODULE(test015) {
    using namespace bbl;
    
    bbl::Class<qux::Bar>()
        .ctor(bbl::Class<qux::Bar>::Ctor<>())
        .m(&qux::Bar::baz)
        ;

    bbl::Class<qux::Foo>()
        .ctor(bbl::Class<qux::Foo>::Ctor<>())
        .m(&qux::Foo::foo)
        .m(&qux::Foo::foo2)
        .m(&qux::Foo::foo3)
        ;
}
