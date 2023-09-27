#include "babble"
#include <string>


// check namespace renaming
BBL_MODULE(std) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);

    bbl::Class<std::string>("String")
        .ctor(bbl::Ctor<std::string, char const*>("ptr"), "from_char_ptr")
    ;
}