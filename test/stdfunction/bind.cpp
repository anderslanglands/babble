#include "babble"

#include "stdfunction.hpp"

BBL_MODULE(stdfunction) {

    bbl::Class<tst::Foo>("Foo")
        .ctor(bbl::Class<tst::Foo>::Ctor<>(), "new")
        .ctor(bbl::Class<tst::Foo>::Ctor<tst::Foo const&>(), "copy")
        .m((tst::Foo& (tst::Foo::*)(tst::Foo const&))
            &tst::Foo::operator=, "op_eq")
        .m(&tst::Foo::set_a)
        .m(&tst::Foo::get_a)
    ;

    bbl::Class<tst::Bar>("Bar")
        .m(tst::Bar::adjust_foo)
    ;

    bbl::Class<tst::FooFn>("FooFn");
    ;

}