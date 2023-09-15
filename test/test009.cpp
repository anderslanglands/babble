#include "babble"
#include "babble-std"

#include <utility>


BBL_MODULE(test_std_pair) {
    bbl::Class<std::pair<int, float>>("StdPairIntFloat");
    BBL_STD_PAIR_FUNCTIONS(int, float, "StdPairIntFloat");
}
