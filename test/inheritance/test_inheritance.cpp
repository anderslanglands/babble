#include "inheritance-c.h"
#include <stdio.h>
#include <string.h>

int msgfn(inh_MessageBase_t const* _this, char const** result, void* userdata) {
    *result = "hello from C implementation!";
    return 0;
}

int msgcount(inh_MessageBase_t const* _this, char const* msg, void* userdata) {
    printf("msgcount %llu\n", strlen(msg));
    return 0;
}

int msg_dtor(inh_MessageBase_Subclass_t* msg, void* userdata) {
    printf("msg_dtor\n");
    return 0;
}

int main() {

    inh_MessageBase_Subclass_t* msgobj;
    inh_MessageBase_Subclass_default(&msgobj, msgfn, nullptr, msgcount, nullptr, msg_dtor, nullptr);

    inh_Printer_t* printer;
    inh_Printer_default(&printer);
    // print_message will delete the Message object passed in
    inh_Printer_print_message(printer, (inh_Message_t*)msgobj);

    inh_Printer_dtor(printer);

    return 0;
}