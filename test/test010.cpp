#include "babble"

using SchemaVersion = unsigned int;

BBL_MODULE(test_bad_binding) {
    bbl::Class<SchemaVersion>();
    bbl::Enum<SchemaVersion>();
}

