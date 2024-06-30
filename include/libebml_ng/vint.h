#ifndef EBML_NG_VINT_H
#define EBML_NG_VINT_H

#include "exceptions.h"
#include "io.h"
#include "ebmlID_t.h"
#include <array>

namespace ebml {
    extern const size_t UNKNOWN;

    // // Detect length of encoded vint based on first character.
    // vintWidth_t vintWidth(char);
    //
    // // Detect minimum width required to encode unsigned long long as a vint.
    // vintWidth_t widthAsVint(unsigned long long);

    // Encode vint, auto-detect vint-width
    vintWidth_t packVint(unsigned long long, char*);

    // Encode vint with provided vint-width.
    void packVint(unsigned long long, vintWidth_t, char*);

    unsigned long long unpackVint(const char* data);

    // Decode vint from char array, providing the size of char array.
    // Raises exception on unexpected end of data.
    // Stores vint-width in provided vintWidth_t reference.
    unsigned long long unpackVint(const char*, size_t, vintWidth_t&);

    // Decode vint from file.
    // Stores vint-width in provided vintWidth_t reference.
    unsigned long long unpackVint(ioBase&, vintWidth_t&);

    // Same as above, but allows for an offset to be specified.
    unsigned long long unpackVint(ioBase&, off_t, vintWidth_t&);

    inline unsigned char vintWidth(char b) {
        static const std::array<unsigned char, 8> masks = {{
            0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
        }};
        unsigned char k;

        for (k = 0; k < 8; ++k) {
            if (b & masks[k]) {
                return k + 1;
            }
        }

        throw ebmlInvalidVint("Variable-length integer cannot start with null byte.");
    }

    inline unsigned char widthAsVint(unsigned long long n) {
        unsigned char k = 0;

        if (n == 0xffffffffffffffff) {
            return 8;
        }

        while (k <= 8) {
            if (n < (1ULL << (7*k)) - 1ULL) {
                return k;
            };
            k++;
        }

        throw std::overflow_error(
            std::string("int too big to convert (") + std::to_string(__LINE__)
            + std::string(":") + std::string(__FILE__) + std::string(")")
        );
    }

    unsigned long long unpackVint(const char* data, unsigned char size);

    inline unsigned long long unpackVint(const char* data) {
        unsigned char size;

        size = vintWidth(data[0]);
        return unpackVint(data, size);
    }

    inline unsigned char packVint(unsigned long long n, char* dest) {
        unsigned char size = widthAsVint(n);
        packVint(n, size, dest);
        return size;
    }
}
#endif
