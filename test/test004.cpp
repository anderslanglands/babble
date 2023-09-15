#include "babble"

namespace qux {
    template <typename T>
    class Foo {
        T _foo;
    public:
        void baz(size_t a);
        void baz(float a);
    };
}

BBL_MODULE(test004) {
    using namespace bbl;
    using namespace qux;

    // distinguish between overloads by casting the method
    bbl::Class<Foo<float>>("FooFloat")
        .m((void (Foo<float>::*)(size_t))&Foo<float>::baz, "baz_int")
        .m((void (Foo<float>::*)(float))&Foo<float>::baz, "baz_float");

    using FooInt = Foo<int>;
    bbl::Class<FooInt>("FooInt")
        .m((void (FooInt::*)(size_t))&FooInt::baz, "baz_int")
        .m((void (FooInt::*)(float))&FooInt::baz, "baz_float");
}
