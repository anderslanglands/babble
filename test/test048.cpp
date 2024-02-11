#include "babble"
#include "babble-std"

#include <stdint.h>

namespace qux {
enum Bar {
    Baz = 0,
    Bum,
    Qux,
};

}

// Test C enum conversion
BBL_MODULE(test048) {
    bbl::Enum<qux::Bar>();

    BBL_STD_PAIR((std::pair<int, qux::Bar>), IntBarPair);
}
