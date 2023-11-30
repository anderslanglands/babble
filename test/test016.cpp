#include "babble"

#include <stdint.h>


namespace qux {
enum class Bar: int64_t {
    Baz = 0,
    Bum,
    Qux,
};

class Foo {
public:
    void take_enum(Bar b) const;
    Bar return_enum();

};

}

// Test C enum conversion
BBL_MODULE(test016) {
    using namespace bbl;
    
    bbl::Enum<qux::Bar>()
        ;

    bbl::Class<qux::Foo>()
        .ctor(bbl::Class<qux::Foo>::Ctor<>())
        .m(&qux::Foo::take_enum)
        .m(&qux::Foo::return_enum)
        ;
}
