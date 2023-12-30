#include <babble>
#include "inheritance.hpp"

namespace bblext {

class MessageBase : public foo::Message {
public:
    auto count_base(char const* msg) -> void {
        foo::Message::count(msg);
    }

    virtual auto count(char const* msg) const -> void override {
        std::cout << "MessageBase is " << strlen(msg) << "chars\n";
    }

    virtual auto msg() const -> char const* override = 0;

    virtual ~MessageBase(){
        std::cout << "~MessageBase()" << std::endl;
    }
};

}

BBL_MODULE(inh) {
    bbl::Class<foo::Message>()
        .m(&foo::Message::msg)
    ;

    bbl::Superclass<bblext::MessageBase>()
        .m(&bblext::MessageBase::msg)
        .m(&bblext::MessageBase::count)
        .m(&bblext::MessageBase::count_base)
    ;

    bbl::Class<foo::Printer>() 
        .ctor(bbl::Class<foo::Printer>::Ctor<>(), "default")
        .m(&foo::Printer::print_message)
    ;
}