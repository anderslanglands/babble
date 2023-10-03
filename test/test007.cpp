#include "babble"

#include <string>
#include <vector>

namespace qux {

class Bar {
public:
    int bar() const noexcept;
    static void baz() noexcept(false);
    static void bum() noexcept(true);
};

// a normal comment
/// function foo
auto foo(Bar&& bar) noexcept -> size_t;

}

BBL_MODULE(test007) {
    bbl::Class<qux::Bar>()
        .m(&qux::Bar::bar)
        .m(&qux::Bar::baz)
        .m(&qux::Bar::bum)
        ;

    bbl::fn(&qux::foo, "myfoo");   
}
