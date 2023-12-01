#include "babble"

namespace qux {
    template <typename T>
    class Foo {
        T _foo;
    public:
        T bar(T const& a);
        T bar_r(T&& a);
    };
}

// FunctionDecl bbl_bind
BBL_MODULE(test002) {
    using namespace bbl;
    using namespace qux;
    bbl::Class<Foo<float>>("FooFloat")
        .m(&Foo<float>::bar)
        .m(&Foo<float>::bar_r);

    using FooInt = Foo<int>;

    bbl::Class<FooInt>("FooInt")
        .m(&FooInt::bar)
        .m(&FooInt::bar_r);

}
