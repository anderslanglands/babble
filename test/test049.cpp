#include "babble"
#include "babble-std"

#include <set>
#include <string>

// Test C enum conversion
BBL_MODULE(test049) {
    bbl::Class<std::string>("String")
        .ignore_all_unbound()
    ;

    BBL_STD_SET(std::set<std::string>, StringSet);
}
