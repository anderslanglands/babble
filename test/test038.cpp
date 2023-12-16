#include <babble>

namespace foo {

class Bar {
public:
    int& get_int();
    Bar const& get_bar();
};

float& get_float();
Bar const& get_bar_ref();

}

BBL_MODULE(test037) {
    bbl::Class<foo::Bar>()
        .m(&foo::Bar::get_int)
        .m(&foo::Bar::get_bar)
    ;

    bbl::fn(&foo::get_float);
    bbl::fn(&foo::get_bar_ref);
}