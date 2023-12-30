#include <iostream>

namespace foo {

class Message {
protected:
    virtual void count(char const* msg) const {
        std::cout << "Message is " << strlen(msg) << " chars\n";
    }

public:
    // Message(){}
    virtual auto msg() const -> char const* = 0;

    virtual auto do_int() const -> int {return 42;}

    virtual ~Message(){
        std::cout << "~Message()" << std::endl;
    }
};

class Printer {
public:
    void print_message(Message* msg) {
        std::cout << "The message is: " << msg->msg() << std::endl;
        delete msg;
    }
};


}