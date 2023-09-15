#include "babble"
#include <functional>


namespace qux {

class Foo {
    int a;
    float b;
public:
    Foo(int a, float b);

    void do_foo(std::function<bool(Foo const& f)> const& fun) const;
};

}

// std function
BBL_MODULE(test018) {
    bbl::Class<qux::Foo>()
        .ctor(bbl::Ctor<qux::Foo, int, float>("a", "b"))
        .m(&qux::Foo::do_foo)
        .opaque_bytes()
        ;

    bbl::Class<std::function<bool(qux::Foo const&)>>("FooFn");
}
