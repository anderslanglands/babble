#include <babble>

namespace foo {

class Incomplete;

class Complete {
    int a;
public:
    void hello(char const* msg) const;
    static Complete* create();
};

}

BBL_MODULE(test034) {
    bbl::ClassIncomplete<foo::Incomplete>()
    ;

    bbl::Class<foo::Complete>()
        .m(&foo::Complete::hello)
        .m(bbl::Wrap(&foo::Complete::create, []() -> foo::Complete* {
            return foo::Complete::create();
        }))
    ;

}