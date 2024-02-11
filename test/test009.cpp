#include "babble"
#include "babble-std"

#include <utility>

namespace bar {
void foo(int a);
}

namespace baz = bar;


BBL_MODULE(test_std_pair) {
    BBL_STD_PAIR((std::pair<int, float>), StdPairIntFloat);

    bbl::fn(&baz::foo);
}
