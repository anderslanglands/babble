#include "babble"

#include <string>

namespace qux {
    template <typename T>
    class Foo {
        T _foo;
    public:
        /// This is the default constructor
        Foo() noexcept;
        /// This constructor takes a single float and is not noexcept
        explicit Foo(T& b) noexcept(false);
        T bar(T const& a);
    };
}

BBL_MODULE(test011) {
    bbl::Class<qux::Foo<float>>("FooFloat")
        // We can't take member function pointers for constructors so they are bound like this:
        .ctor(bbl::Class<qux::Foo<float>>::Ctor<>())
        .ctor(bbl::Class<qux::Foo<float>>::Ctor<float&>("f"), "with_float")
        .m(&qux::Foo<float>::bar);
}
