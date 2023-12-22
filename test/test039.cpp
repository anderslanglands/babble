#include <babble>
#include <string>

namespace foo {

class Base {
public:
    virtual float hello(std::string& str, int& a, float* b) const = 0;
    virtual int hello2(std::string& str) const;
    void nonvirtual();
    virtual ~Base(){}
};

}

BBL_MODULE(test039) {
    bbl::Class<foo::Base>()
        .superclass()
        .m(&foo::Base::hello)
        .m(&foo::Base::hello2)
        .m(&foo::Base::nonvirtual)
    ;

    bbl::Class<std::string>("string");
}
