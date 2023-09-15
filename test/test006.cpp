#include "babble"

#include <string>
#include <vector>
//#include <NO>
/*#include NO NOTE ME EITHER*/

namespace qux {

enum FooEnum {
    Zero = 0,
    One,
    Two,
};

enum class BarEnum: int64_t {
    MinusOne = -1,
    Zero,
    Two = FooEnum::Two,
    Three = 3,
    ThreeAsWell = Three,
};

}

BBL_MODULE(test006a) {
    bbl::Enum<qux::FooEnum>("Foo");
}

BBL_MODULE(test006b) {
    bbl::Enum<qux::BarEnum>("Bar");
}
