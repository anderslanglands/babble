#include "babble"


namespace qux {

struct Vec3f {
    float x;
    float y = 0;
    float z{0.0};

    float length() const;
    float dist(Vec3f const& other) const;
};

class Foo {
    int a;
    float b;
public:
    Foo(int a, float b);

    void do_foo(Foo const& other) const;
    void do_foo2(Foo* other);
};

class Bar {
    int a;
    float b;
public:
    Bar(int a, float b);
};

}

// Test bind kinds and constructors
BBL_MODULE(test017) {
    bbl::Class<qux::Vec3f>()
        .value_type()
        .ctor(bbl::Class<qux::Vec3f>::Ctor<>())
        .m(&qux::Vec3f::length)
        .m(&qux::Vec3f::dist)
        .f(&qux::Vec3f::x)
        .f(&qux::Vec3f::y)
        .f(&qux::Vec3f::z)
        ;

    bbl::Class<qux::Foo>()
        .ctor(bbl::Class<qux::Foo>::Ctor<int, float>("a", "b"))
        .m(&qux::Foo::do_foo)
        .m(&qux::Foo::do_foo2)
        ;

    bbl::Class<qux::Bar>()
        .ctor(bbl::Class<qux::Bar>::Ctor<int, float>("a", "b"))
        ;
}
