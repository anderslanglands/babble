#if defined(__clang__)

#include "babble"
#include "babble-std"

#include <pxr/base/tf/token.h>
#include <pxr/base/tf/type.h>

using Token = PXR_NS::TfToken;

BBL_MODULE(tf) {
    bbl::rename_namespace("PXR_NS::", "pxr");
    bbl::prepend_module_name(true);


    // clang-format off

    bbl::Class<PXR_NS::TfToken>("Token")
        .opaque_ptr()
        .ctor(bbl::Ctor<Token>(), "new")
        .m((Token& (Token::*)(Token const&))&Token::operator=, "op_assign")
        .m(&Token::GetText)
        ;

    bbl::Class<PXR_NS::TfType>("Type")
        ;

    bbl::Class<PXR_NS::TfTokenVector>("TokenVector")
        BBL_STD_VECTOR_METHODS(PXR_NS::TfToken)
        ;
}


#endif

