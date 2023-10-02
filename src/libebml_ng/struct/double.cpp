#ifndef EBML_NG_STRUCT_DOUBLE_CPP
#define EBML_NG_STRUCT_DOUBLE_CPP

#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct.tpp"

namespace ebml {
    FIXED_SIZE(float, 4)
    FIXED_SIZE(double, 8)

    DEF_PACK_ALT(float) {
        memcpy(dest, &value, sizeof(float));

        if (_is_littleendian) {
            _reverse(dest, 0, sizeof(float) - 1);
        }

        return sizeof(float);
    }

    DEF_PACK_ALT(double) {
        memcpy(dest, &value, sizeof(double));

        if (_is_littleendian) {
            _reverse(dest, 0, sizeof(double) - 1);
        }

        return sizeof(double);
    }

    DEF_UNPACK(float) {
        float ret = 0;

        if (size == 0) {return 0;}

        if (size != 4) {
            throw std::invalid_argument("unpack<float>: invalid value for size");
        }

        memcpy(&ret, src, sizeof(float));

        if (_is_littleendian) {
            _reverse((char*)&ret, 0, sizeof(float) - 1);
        }

        return ret;
    }

    DEF_UNPACK(double) {
        double ret = 0;

        if (size == 0) {return 0;}

        if (size == 4) {return unpack<float>(src, size);}

        if (size != 8) {
            throw std::invalid_argument("unpack<double>: invalid value for size");
        }

        memcpy(&ret, src, sizeof(double));

        if (_is_littleendian) {
            _reverse((char*)&ret, 0, sizeof(double) - 1);
        }

        return ret;
    }

    INST_TEMPLATES_ALT(float)
    INST_TEMPLATES_ALT(double)
}
#endif
