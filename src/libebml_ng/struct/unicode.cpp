#ifndef EBML_NG_STRUCT_UNICODE_CPP
#define EBML_NG_STRUCT_UNICODE_CPP

#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/struct.tpp"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/ebmlElement.h"

namespace ebml {
    static const unsigned long long sizes[4] = {1LL << 7, 1LL << 11, 1LL << 16, 1LL << 21};

    // static const unsigned char fcp[4] = {0b00000000, 0b11000000, 0b11100000, 0b11110000};
    // static const unsigned char fcm[4] = {0b10000000, 0b11100000, 0b11110000, 0b11111000};
    // static unsigned char overlong[4][3] = {
    //     {0},
    //     {0b00011110, 0},
    //     {0b00001111, 0b00100000, 0},
    //     {0b00000111, 0b00110000, 0},
    // };

    static const unsigned char fcp[4] = {0x00, 0xC0, 0xE0, 0xF0};
    static const unsigned char fcm[4] = {0x80, 0xE0, 0xF0, 0xF8};

    static const unsigned char overlong[4][3] = {
        {0x00},
        {0x1E, 0x00},
        {0x0F, 0x20, 0x00},
        {0x07, 0x30, 0x00},
    };

    DEF_SIZE(std::wstring) {
        const wchar_t* data = value.c_str();
        size_t usize = value.size();
        off_t uk = 0;
        wchar_t u;
        unsigned char j;
        size_t dsize = 0;

        while (uk < usize) {
            u = data[uk];

            if (u > 0x10ffff) {
                throw unicodeEncodeError("character out of range (not in range(0x110000))", value, uk, uk+1);
                // throw std::invalid_argument("chr() arg not in range(0x110000)");
            }

            if (0xd800 <= u and u < 0xe000) {
                throw unicodeEncodeError("surrogates not allowed", value, uk, uk+1);
            }

            j = 0;

            while (j < 4) {
                if (u < sizes[j]) {
                    dsize += j + 1;
                    break;
                }

                j++;
            }

            uk++;
        }

        return dsize;
    }

    DEF_PACK(std::wstring) {
        const wchar_t* data = value.c_str();
        size_t usize = value.size();
        size_t uk = 0;
        size_t dk = 0;
        wchar_t u;
        unsigned char j;
        unsigned char charWidth;
        unsigned char o;

        while (uk < usize) {
            u = data[uk];

            if (u > 0x10ffff) {
                throw unicodeEncodeError("character out of range (not in range(0x110000))", value, uk, uk+1);
            }

            if (0xd800 <= u and u < 0xe000) {
                throw unicodeEncodeError("surrogates not allowed", value, uk, uk+1);
            }

            j = 0;

            while (j < 4) {
                if (u < sizes[j]) {
                    break;
                }

                j++;
            }

            charWidth = j + 1;
            o = 6*j;

            dest[dk] = (
                ((u >> o) & ~fcm[j])
                | fcp[j]);

            if (size < dk + j + 1) {
                throw ebmlEncodeError("Encoded UTF-8 string exceeds expected size.");
            }

            o -= 6;
            j = 1;

            while (j < charWidth) {
                dest[dk + j] = ((u >> o) & 0b00111111) | 0b10000000;
                j++;
                o -= 6;
            }
            dk += charWidth;
            uk++;
        }

        return dk;
    }

    unsigned char utf8_char_width(unsigned char d) {
        unsigned char k = 0;

        while (k < 4) {
            if ((d & fcm[k]) == fcp[k]) {
                return k + 1;
            }

            k++;
        }

        return 0;
    }

    bool detect_overlong_encode(const char* src, const unsigned char* masks) {
        if (masks[0] == 0) {
            return false;
        }

        unsigned int k = 0;
        while (masks[k] != 0) {
            if ((src[k] & masks[k]) != 0) {
                return false;
            }
            k++;
        }
        return true;
    }

    DEF_UNPACK(std::wstring) {
        std::wstring str;
        size_t uk = 0;
        size_t dk = 0;
        wchar_t u;
        unsigned char d;
        unsigned char charWidth;
        unsigned char o;
        unsigned char m;
        unsigned char j;

        str.resize(size);
        wchar_t* data = &str[0];

        while (dk < size) {
            d = src[dk];

            charWidth = utf8_char_width(d);

            if (charWidth == 0) {
                throw unicodeDecodeError("invalid start byte", DECODE_ERR_DEFAULT, dk, dk+1, std::string(src, size));
            }

            if ((dk + charWidth) > size) {
                throw unicodeDecodeError("unexpected end of data", DECODE_ERR_DEFAULT, dk, size, std::string(src, size));
            }

            if (detect_overlong_encode(src + dk, overlong[charWidth-1])) {
                throw unicodeDecodeError("overlong encoding detected",DECODE_ERR_DEFAULT, dk, dk + charWidth, std::string(src, size));
            }

            if ((static_cast<unsigned char>(src[dk]) == 0xed) and (static_cast<unsigned char>(src[dk + 1]) & 0x20)) {
                throw unicodeDecodeError("surrogates not allowed", DECODE_ERR_DEFAULT, dk, dk + charWidth, std::string(src, size));
            }

            o = 6*(charWidth - 1);

            m = ~fcm[charWidth - 1];
            u = (wchar_t)(d & m) << o;
            j = 1;
            o -= 6;

            while (j < charWidth) {
                d = src[dk + j];

                if ((d & 0b11000000) != 0b10000000) {
                    throw unicodeDecodeError("invalid continuation byte", DECODE_ERR_DEFAULT, dk, dk+j, std::string(src, size));
                }

                m = 0b00111111;
                u |= (wchar_t)(d & m) << o;
                j++;
                o -= 6;
            }

            data[uk] = u;
            dk += charWidth;
            ++uk;
        }

        str.resize(uk);
        return str;
    }

    INST_TEMPLATES(std::wstring)
}
#endif
