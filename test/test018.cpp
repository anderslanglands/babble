#include "babble"
#include <functional>


namespace qux {

class Bar {
    int a;
    float b;
public:
    void bar();
};

class Foo {
    int a;
    float b;
public:
    Foo(int a, float b);

    void do_foo(std::function<Foo(Foo const&, int, float)> const& fun) const;
    void do_foo2(std::function<Foo const&(Foo const&, int, float)> const& fun) const;
};

}

// std function
BBL_MODULE(test018) {
    bbl::Class<qux::Foo>("Foo")
        .ctor(bbl::Class<qux::Foo>::Ctor<int, float>("a", "b"))
        .m(&qux::Foo::do_foo)
        .m(&qux::Foo::do_foo2)
        ;

    bbl::Class<std::function<qux::Foo(qux::Foo const&, int, float)>>("FooFn");
    bbl::Class<std::function<qux::Foo const& (qux::Foo const&, int, float)>>("Foo2Fn");
}

#if 0

void take_foofn(void (fn*)(qux_Foo_t const*, bool*)) {
    auto bbl_lamba_wrapper00 = [&](qux::Foo const& param00) -> bool {
        qux::Foo result;
        fn(&param00, &result);
        return result;
    };

    cpp::takefoofn(bbl_lambda_wrapper00);
}


#endif
