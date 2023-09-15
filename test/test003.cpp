#include "babble"

namespace qux {
    class Baz {

    };

    template <typename T>
    class Foo {
        T _foo;
    public:
        T bar(T const& a);
        void baz(Baz* b);
    };
}

BBL_MODULE(test003) {
    using namespace bbl;
    using namespace qux;

    bbl::Class<Foo<float>>("FooFloat")
        .opaque_bytes()
        .m(&Foo<float>::bar)
        .m(&Foo<float>::baz);

    using FooInt = Foo<int>;
    bbl::Class<FooInt>("FooInt")
        .value_type()
        .m(&FooInt::bar)
        .m(&FooInt::baz);

    bbl::Class<Baz>();

}
