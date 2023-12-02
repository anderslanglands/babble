#include <babble>

namespace foo {

class Bar {
public:
    void baz(int b) const&;
};

}

BBL_MODULE(test037) {
    bbl::Class<foo::Bar>()
        .m(&foo::Bar::baz)
    ;
}