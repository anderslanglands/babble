#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__clang__)
#  define BBL_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#  define BBL_ALIGN(x) __declspec(align(x))
#else
#  error "Unknown compiler; can't define ALIGN"
#endif

#include <stddef.h>
#include <exception>
#include <thread>
#include <string>

static thread_local std::string __bbl_error_message;

extern "C" {

using test_std_vector_StdVectorInt_t = std::vector<std::pair<int, float>>;
using test_std_vector_StdPairIntFloat_t = std::pair<int, float>;

int test_std_vector_StdVectorInt_data(test_std_vector_StdVectorInt_t* _this, test_std_vector_StdPairIntFloat_t** _result) {
    *_result = _this->data();
    return 0;
}

int test_std_vector_StdVectorInt_data_const(test_std_vector_StdVectorInt_t const* _this, test_std_vector_StdPairIntFloat_t const** _result) {
    *_result = _this->data();
    return 0;
}

int test_std_vector_StdVectorInt_empty(test_std_vector_StdVectorInt_t const* _this, bool* _result) {
    *_result = _this->empty();
    return 0;
}

int test_std_vector_StdVectorInt_size(test_std_vector_StdVectorInt_t const* _this, size_t* _result) {
    *_result = _this->size();
    return 0;
}

int test_std_vector_StdVectorInt_max_size(test_std_vector_StdVectorInt_t const* _this, size_t* _result) {
    *_result = _this->max_size();
    return 0;
}

int test_std_vector_StdVectorInt_reserve(test_std_vector_StdVectorInt_t* _this, size_t const _Newcapacity) {
    try {
        _this->reserve(_Newcapacity);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test_std_vector_StdVectorInt_capacity(test_std_vector_StdVectorInt_t const* _this, size_t* _result) {
    *_result = _this->capacity();
    return 0;
}

int test_std_vector_StdVectorInt_clear(test_std_vector_StdVectorInt_t* _this) {
    _this->clear();
    return 0;
}

int test_std_vector_StdVectorInt_push_back(test_std_vector_StdVectorInt_t* _this, test_std_vector_StdPairIntFloat_t const* _Val) {
    try {
        _this->push_back(*_Val);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test_std_vector_StdVectorInt_pop_back(test_std_vector_StdVectorInt_t* _this) {
    _this->pop_back();
    return 0;
}

int test_std_vector_StdVectorInt_resize(test_std_vector_StdVectorInt_t* _this, size_t const _Newsize) {
    try {
        _this->resize(_Newsize);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test_std_vector_StdVectorInt_resize_with(test_std_vector_StdVectorInt_t* _this, size_t const _Newsize, test_std_vector_StdPairIntFloat_t const* _Val) {
    try {
        _this->resize(_Newsize, *_Val);
        return 0;
    } catch (std::exception& e) {
        __bbl_error_message = e.what();
        return 1;
    }
}

int test_std_vector_StdVectorInt_op_index(test_std_vector_StdVectorInt_t const* _this, size_t const _Pos, test_std_vector_StdPairIntFloat_t const** _result) {
    *_result = &_this->operator[](_Pos);
    return 0;
}

int test_std_vector_StdVectorInt_default(test_std_vector_StdVectorInt_t** _result) {
    *_result = new std::vector<std::pair<int, float>>();
    return 0;
}

int test_std_vector_StdVectorInt_dtor(test_std_vector_StdVectorInt_t* _this) {
    delete _this;
    return 0;
}

int test_std_vector_StdPairIntFloat_dtor(test_std_vector_StdPairIntFloat_t* _this) {
    delete _this;
    return 0;
}

} // extern "C"
