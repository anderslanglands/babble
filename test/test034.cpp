#include <babble>

namespace foo {

class Incomplete;

class Complete1 {
    int a;
};

class Complete2 {
    int a;
};

}

BBL_MODULE(test034) {
    bbl::ClassIncomplete<foo::Incomplete>()
    ;

    bbl::Class<foo::Complete1>();

    bbl::Class<foo::Complete2>();
}