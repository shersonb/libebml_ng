#ifndef EBML_NG_STRUCT_CPP
#define EBML_NG_STRUCT_CPP
#include "libebml_ng/struct.h"
#include "libebml_ng/exceptions.h"
#include <stdio.h>

namespace ebml {
    unsigned long long __one = 1;
    bool _is_littleendian = (((char*)&__one)[0] == 1);

    void _reverse(char* s, unsigned int j, unsigned int k) {
        while (j < k) {
            s[j] ^= s[k];
            s[k] ^= s[j];
            s[j] ^= s[k];

            j++;
            k--;
        };
    }

//     size_t pack(const unsigned long long& n, size_t size, char* dest) {
//         char* as_chars;
//
//         if (size == 0 or size > 8) {
//             throw std::invalid_argument(make_exc_msg("pack: invalid value for size", __LINE__, __FILE__));
//         };
//
//         if (size != 8 and (n >> (8*size)) != 0) {
//             throw std::overflow_error(make_exc_msg("pack: int too big to convert", __LINE__, __FILE__));
//         };
//
//         as_chars = (char*)&n;
//
//         if (_is_littleendian) {
//             memcpy(dest, as_chars, size);
//             _reverse(dest, 0, size - 1);
//         } else {
//             memcpy(dest, as_chars + (8 - size), size);
//         }
//
//         return size;
//     }

//     size_t pack(const long long& n, size_t size, char* dest) {
//         char* as_chars;
//
//         if (size == 0 or size > 8) {
//             throw std::invalid_argument(make_exc_msg("pack: invalid value for size", __LINE__, __FILE__));
//         };
//
//         if (size != 8) {
//             if (n > 0 && (n >> (8*size)) != 0) {
//                 throw std::overflow_error(make_exc_msg("pack: int too big to convert", __LINE__, __FILE__));
//             };
//             if (n < 0 && (n >> (8*size)) != -1) {
//                 throw std::overflow_error(make_exc_msg("pack: int too big to convert", __LINE__, __FILE__));
//             };
//         };
//
//         as_chars = (char*)&n;
//
//         if (_is_littleendian) {
//             memcpy(dest, as_chars, size);
//             _reverse(dest, 0, size - 1);
//         } else {
//             memcpy(dest, as_chars + (8 - size), size);
//         }
//
//         return size;
//     }

//     size_t pack(double n, char* dest) {
//         char* as_chars;
//
//         as_chars = (char*)&n;
//         memcpy(dest, as_chars, sizeof(double));
//
//         if (_is_littleendian) {
//             _reverse(dest, 0, sizeof(double) - 1);
//         }
//
//         return sizeof(double);
//     }

//     size_t pack(const float& n, char* dest) {
//         char* as_chars;
//
//         as_chars = (char*)&n;
//         memcpy(dest, as_chars, sizeof(float));
//
//         if (_is_littleendian) {
//             _reverse(dest, 0, sizeof(float) - 1);
//         }
//
//         return sizeof(float);
//     }

//     template<>
//     unsigned long long unpack<unsigned long long>(const char* s, size_t size) {
//         unsigned long long ret = 0;
//         char* as_chars;
//
//         if (size == 0) {
//             return 0;
//         };
//
//         if (size > 8) {
//             throw std::invalid_argument(make_exc_msg(
//                 "unpack[unsigned long long]: invalid value for size", __LINE__, __FILE__));
//         };
//
//         as_chars = (char*)&ret;
//
//         if (_is_littleendian) {
//             memcpy(as_chars, s, size);
//             _reverse(as_chars, 0, size - 1);
//         } else {
//             memcpy(as_chars + (8 - size), s, size);
//         }
//
//         return ret;
//     }

//     template<>
//     long long unpack<long long>(const char* s, size_t size) {
//         long long ret = 0;
//         char* as_chars;
//
//         if (size == 0) {
//             return 0;
//         };
//
//         if (size > 8) {
//             throw std::invalid_argument(make_exc_msg(
//                 "unpack[long long]: invalid value for size", __LINE__, __FILE__));
//         };
//
//         as_chars = (char*)&ret;
//
//         if (s[0] & -128) {
//             ret = -1ll;
//         } else {
//             ret = 0;
//         }
//
//         if (_is_littleendian) {
//             memcpy(as_chars, s, size);
//             _reverse(as_chars, 0, size - 1);
//         } else {
//             memcpy(as_chars + (8 - size), s, size);
//         }
//
//         return ret;
//     }

//     template<>
//     float unpack<float>(const char* s, size_t size) {
//         float ret = 0;
//         char* as_chars;
//
//         if (size == 0) {return 0;}
//
//         if (size != 4) {
//             throw std::invalid_argument(make_exc_msg(
//                 "unpack[float]: invalid value for size", __LINE__, __FILE__));
//
//         }
//
//         as_chars = (char*)&ret;
//
//         memcpy(as_chars, s, sizeof(float));
//
//         if (_is_littleendian) {
//             _reverse(as_chars, 0, sizeof(float) - 1);
//         }
//
//         return ret;
//     }
//
//     template<>
//     double unpack<double>(const char* s, size_t size) {
//         double ret = 0;
//         char* as_chars;
//
//         if (size == 0) {return 0;}
//
//         if (size == 4) {return unpack<float>(s, size);}
//
//         if (size != 8) {
//             throw std::invalid_argument(make_exc_msg(
//                 "unpack[float]: invalid value for size", __LINE__, __FILE__));
//
//         }
//
//         as_chars = (char*)&ret;
//
//         memcpy(as_chars, s, sizeof(double));
//
//         if (_is_littleendian) {
//             _reverse(as_chars, 0, sizeof(double) - 1);
//         }
//
//         return ret;
//     }
    template<typename T>
    size_t pack(const T& value, char* dest) {
        size_t packedSize = size(value);
        return pack(value, packedSize, dest);
    }

    template<typename T>
    size_t pack(const T& value, size_t size, char* dest) {
        size_t packedSize = pack(value, dest);
        if (packedSize > size) {
            // The packed data size exceeds the provided size
            throw std::runtime_error("Packed data size exceeds buffer size");
        }
        return packedSize;
    }
}
#endif
