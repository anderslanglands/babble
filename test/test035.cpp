#include <babble>

namespace OIIO {

class ImageCache {
public:
    ImageCache& operator=(ImageCache const&);

    void hello();
};

BBL_MODULE(test035) {
    bbl::Class<OIIO::ImageCache>()
        .m(&OIIO::ImageCache::hello)
        .m(&OIIO::ImageCache::operator=, "op_assign")
    ;
}

}