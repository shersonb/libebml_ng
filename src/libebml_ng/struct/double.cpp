#ifndef EBML_NG_STRUCT_DOUBLE_CPP
#define EBML_NG_STRUCT_DOUBLE_CPP
// #include "libebml_ng/struct.cpp"
#include "libebml_ng/struct/double.h"

namespace ebml {
    size_t size(const float&) {
        return 4;
    }

    size_t size(const double&) {
        return 8;
    }

    size_t pack(const float& x, char* dest) {
        char* as_chars;

        as_chars = (char*)&x;
        memcpy(dest, as_chars, sizeof(float));

        if (_is_littleendian) {
            _reverse(dest, 0, sizeof(float) - 1);
        }

        return sizeof(float);
    }

    size_t pack(const double& x, char* dest) {
        char* as_chars;

        as_chars = (char*)&x;
        memcpy(dest, as_chars, sizeof(double));

        if (_is_littleendian) {
            _reverse(dest, 0, sizeof(double) - 1);
        }

        return sizeof(double);
    }

    template<>
    float unpack<float>(const char* s, size_t size) {
        float ret = 0;
        char* as_chars;

        if (size == 0) {return 0;}

        if (size != 4) {
            throw std::invalid_argument("unpack<float>: invalid value for size");
        }

        as_chars = (char*)&ret;

        memcpy(as_chars, s, sizeof(float));

        if (_is_littleendian) {
            _reverse(as_chars, 0, sizeof(float) - 1);
        }

        return ret;
    }

    template<>
    double unpack<double>(const char* s, size_t size) {
        double ret = 0;
        char* as_chars;

        if (size == 0) {return 0;}

        if (size == 4) {return unpack<float>(s, size);}

        if (size != 8) {
            throw std::invalid_argument("unpack<double>: invalid value for size");
        }

        as_chars = (char*)&ret;

        memcpy(as_chars, s, sizeof(double));

        if (_is_littleendian) {
            _reverse(as_chars, 0, sizeof(double) - 1);
        }

        return ret;
    }
}
#endif
