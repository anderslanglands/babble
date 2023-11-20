#include "babble"
#include "babble-std"

#include <utility>

namespace bar {
void foo(int a);
}

namespace baz = bar;


BBL_MODULE(test_std_pair) {
    bbl::Class<std::pair<int, float>>("StdPairIntFloat")
        .opaque_ptr();
    BBL_STD_PAIR_FUNCTIONS(int, float, "StdPairIntFloat");

    bbl::fn(&baz::foo);
}
