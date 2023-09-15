#include "babble"

namespace qux {
    template <typename T>
    class Base {
    public:
        using size_type = size_t;

        size_type baz(T* b);
    };

    // Check that we can handle CRTP
    template <typename T>
    class Foo : public Base<Foo<T>> {
        T _foo;
    public:
        // This was a nasty little construct in OpenTimelineIO
        using size_type = typename Base<Foo<T>>::size_type;

        size_type bar(T const& a);
    };
}

BBL_MODULE(test005) {
    using namespace bbl;
    using namespace qux;

    bbl::Class<Foo<float>>("FooFloat")
        .m(&Foo<float>::bar)
        .m(&Foo<float>::baz);

    // bbl::Class<Baz>();

}
