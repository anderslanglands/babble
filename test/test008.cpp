#include "babble"
#include "babble-std"

#include <string>
#include <vector>

BBL_MODULE(test_std_vector) {
    bbl::Class<std::vector<std::pair<int, float>>>("StdVectorInt")
        // Note that we wrap the type in `()` here because it contains commas
        BBL_STD_VECTOR_METHODS((std::pair<int, float>));

    // bbl::Class<std::pair<int, float>>("StdPairIntFloat");
}
