#include <babble>
#include <babble-std>
#include <string>
#include <utility>


// Test binding a pair in one shot with macros
BBL_MODULE(test047) {
#if 1
    BBL_STD_PAIR((std::pair<float, int>), FloatIntPair);
#else    
    bbl::Class<std::pair<float, int>>("FloatIntPair");
    bbl::fn([](std::pair<float, int> const& p) -> std::pair<float, int>::first_type const& {
        return p.first;
    }, "FloatIntPair_first");
    bbl::fn([](std::pair<float, int> const& p) -> std::pair<float, int>::second_type const& {
        return p.second;
    }, "FloatIntPair_second");
#endif
}
