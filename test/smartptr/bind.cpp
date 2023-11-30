#include "babble"

#include "smartptr.hpp"

BBL_MODULE(bar) {
    bbl::Class<Foo>("Foo")
        .m(&Foo::create)
        .m(&Foo::get_foo)
    ;

    bbl::Class<FooPtr>("FooPtr")
        .ctor(bbl::Class<FooPtr>::Ctor<>(), "new")
        .smartptr_to<Foo>()
        ;
}