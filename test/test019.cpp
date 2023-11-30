#include "babble"
#include <functional>


namespace QUX_NS {

class BarFoo {
    int a;
    float b;
public:
    BarFoo(int a, float b);
};

enum BarEnum {
    BarEnumZero = 0,
    BarEnumOne,
    BarEnumTwo
};

void bar_fn(BarFoo const& a, BarEnum b, BarEnum* c);

class BazFoo {
    int a;
    float b;
public:
    BazFoo(int a, float b);
};

enum BazEnum {
    BazEnumZero = 0,
    BazEnumOne,
    BazEnumTwo
};

void baz_fn(BazFoo const& a, BazEnum b, BazEnum& c);

}

// check namespace renaming
BBL_MODULE(bar) {
    bbl::rename_namespace("QUX::", "qux");
    bbl::prepend_module_name(true);

    bbl::Class<QUX_NS::BarFoo>("Foo")
        .ctor(bbl::Class<QUX_NS::BarFoo>::Ctor<int, float>("a", "b"))
        ;

    bbl::Enum<QUX_NS::BarEnum>("Enum")
        .prefix("");

    bbl::fn(&QUX_NS::bar_fn);
}

BBL_MODULE(baz) {
    bbl::rename_namespace("QUX::", "qux");
    bbl::prepend_module_name(false);

    bbl::Class<QUX_NS::BazFoo>("Foo")
        .ctor(bbl::Class<QUX_NS::BazFoo>::Ctor<int, float>("a", "b"))
        ;

    bbl::Enum<QUX_NS::BazEnum>("Enum")
        .prefix("BAZ")
        ;

    bbl::fn(&QUX_NS::baz_fn);
}

