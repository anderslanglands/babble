#include "babble"

#include <string>

namespace qux {
    template <typename T>
    class Foo {
        T _foo;
    public:
        Foo() noexcept;
        explicit Foo(T& b) noexcept(false);
        T bar(T const& a);
    };
}

BBL_MODULE(test011) {
    bbl::Class<qux::Foo<float>>("FooFloat")
        // We can't take member function pointers for constructors so they are bound like this:
        .ctor(bbl::Ctor<qux::Foo<float>>())
        .ctor(bbl::Ctor<qux::Foo<float>, float&>("f"), "with_float")
        .m(&qux::Foo<float>::bar);
}
