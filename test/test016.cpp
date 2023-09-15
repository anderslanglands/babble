#include "babble"


namespace qux {
enum class Bar: int64_t {
    Baz = 0,
    Bum,
    Qux,
};

class Foo {
public:
    Foo const& foo(Bar b) const;
};

}

// Test C enum conversion
BBL_MODULE(test016) {
    using namespace bbl;
    
    bbl::Enum<qux::Bar>()
        ;

    bbl::Class<qux::Foo>()
        .ctor(bbl::Ctor<qux::Foo>())
        .m(&qux::Foo::foo)
        ;
}
