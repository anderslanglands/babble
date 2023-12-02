#include <babble>

namespace OIIO {

class ustring {
public:
    bool operator<(const ustring& x) const noexcept { return true; }
};

}

BBL_MODULE(test036) {
    bbl::Class<OIIO::ustring>()
        .m(&OIIO::ustring::operator<, "op_lt")
    ;
}