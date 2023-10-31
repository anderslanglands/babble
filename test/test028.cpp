#include "babble"

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
    static void adjust_foo(FooFn fn);
};

int free_adjust_foo(FooFn fn);


}
BBL_MODULE(test028) {
    bbl::Class<tst::Foo>("Foo")
    ;

    bbl::Class<tst::FooFn>()
    ;

    bbl::Class<tst::Bar>("Bar")
        .m(&tst::Bar::adjust_foo)
    ;

    bbl::fn(&tst::free_adjust_foo);
}
