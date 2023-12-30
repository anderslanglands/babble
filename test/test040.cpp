#include <babble>
#include <string>

namespace foo {

class Base {
public:
    Base();
    Base(std::string foo, std::string& foo_ref, std::string* foo_ptr);
    Base(int foo, int& foo_ref, int* foo_ptr);

    virtual float hello(std::string& str, int& a, float* b) const;
    virtual int hello2(std::string& str) const;
    void nonvirtual();
    virtual ~Base(){}
};

class Derived : public Base {

};

}

BBL_MODULE(test040) {
    bbl::Class<foo::Base>()
        .ctor(bbl::Class<foo::Base>::Ctor<>(), "default")
        .ctor(bbl::Class<foo::Base>::Ctor<std::string, std::string&, std::string*>("foo", "foo_ref", "foo_ptr"), "from_str")
        .ctor(bbl::Class<foo::Base>::Ctor<int, int&, int*>("foo", "foo_ref", "foo_ptr"), "from_int")
        .m(&foo::Base::hello)
        .m(&foo::Base::hello2)
        .m(&foo::Base::nonvirtual)
    ;

    bbl::Class<foo::Derived>();

    bbl::Class<std::string>("string");
}
