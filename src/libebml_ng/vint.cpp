#ifndef EBML_NG_VINT_CPP
#define EBML_NG_VINT_CPP

#include <string.h>
#include <stdio.h>
#include <array>

#include "libebml_ng/struct.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/repr.h"

namespace ebml {
    const size_t UNKNOWN = 0xffffffffffffffff;

    // unsigned char vintWidth(char b) {
    //     static const std::array<unsigned char, 8> masks = {{
    //         0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
    //     }};
    //     unsigned char k;
    //
    //     for (k = 0; k < 8; ++k) {
    //         if (b & masks[k]) {
    //             return k + 1;
    //         }
    //     }
    //
    //     throw ebmlInvalidVint("Variable-length integer cannot start with null byte.");
    // }
    //
    // unsigned char widthAsVint(unsigned long long n) {
    //     unsigned char k = 0;
    //
    //     if (n == 0xffffffffffffffff) {
    //         return 8;
    //     }
    //
    //     while (k <= 8) {
    //         if (n < (1ULL << (7*k)) - 1ULL) {
    //             return k;
    //         };
    //         k++;
    //     }
    //
    //     throw std::overflow_error(
    //         std::string("int too big to convert (") + std::to_string(__LINE__)
    //         + std::string(":") + std::string(__FILE__) + std::string(")")
    //     );
    // }

    void packVint(unsigned long long n, unsigned char size, char* dest) {
        if ((size == 0) or (size > 8)) {
            throw std::invalid_argument(
                std::string("packVint: invalid value for size (") + std::to_string(__LINE__)
                + std::string(":") + std::string(__FILE__) + std::string(")")
            );
        }

        if (n == 0xffffffffffffffff) {
            pack((1ULL << (7*size + 1)) - 1ULL, size, dest);
            return;
        }

        if ((1ULL << (7*size)) - 1ULL <= n) {
            throw std::overflow_error(
                std::string("int too big to convert (") + std::to_string(__LINE__)
                + std::string(":") + std::string(__FILE__) + std::string(")")
            );
        };

        pack((1ULL << (7*size)) | n, size, dest);
    }

    // unsigned char packVint(unsigned long long n, char* dest) {
    //     unsigned char size = widthAsVint(n);
    //     packVint(n, size, dest);
    //     return size;
    // }

    unsigned long long unpackVint(const char* data, unsigned char size) {
        unsigned long long val;

        val = unpack<unsigned long long>(data, size);
        val &= ~(1ULL << (7*size));

        if (val == (1ULL << (7*size)) - 1ULL) {
            return 0xffffffffffffffff;
        };

        return val;
    }

    // unsigned long long unpackVint(const char* data) {
    //     unsigned char size;
    //
    //     size = vintWidth(data[0]);
    //     return unpackVint(data, size);
    // }

    unsigned long long unpackVint(const char* data, size_t dataSize, unsigned char& vintw) {
        unsigned char size;

        if (dataSize == 0) {
            throw ebmlUnexpectedEndOfData("unpackVint(const char*, size_t, unsigned char&): No data.");
        }

        size = vintWidth(data[0]);

        if (dataSize < size) {
            throw ebmlUnexpectedEndOfData("unpackVint(const char*, size_t, unsigned char&): Unexpected end of data.");
        }

        vintw = size;
        return unpackVint(data, size);
    }

    unsigned long long unpackVint(ioBase& file, unsigned char& vintw) {
        char buffer[8];
        unsigned char width;
        unsigned char readsize;
        off_t offset = file.tell();

        if (file.read(buffer, 1) == 0) {
            /*Use vint[0] = 0 to indicate end of stream.*/
            vintw = 0;
            return 0;
        }

        width = vintWidth(buffer[0]);

        if (width == 0) {
            throw ebmlInvalidVint("unpackVint(ioBase*, unsigned char&): Invalid start byte for vint.", DECODE_ERR_DEFAULT, offset);
        }

        if (width > 1) {
            readsize = file.read(buffer + 1, width - 1);

            if (readsize + 1 < width) {
                throw ebmlUnexpectedEndOfData("unpackVint(ioBase*, unsigned char&): Unexpected end of data while attempting to read vint.",
                                              DECODE_ERR_DEFAULT, offset);
            }
        }
        vintw = width;

        try {
            return unpackVint(buffer, width);
        } catch (ebmlDecodeError& e) {
            e.add_to_offset(offset);
            throw e;
        }
    }

    unsigned long long unpackVint(ioBase& file, off_t offset, unsigned char& vintw) {
        char buffer[8];
        unsigned char width;
        unsigned char readsize;

        if (file.read(buffer, offset, 1) == 0) {
            // Use vintw = 0 to indicate end of stream.
            vintw = 0;
            return 0;
        }

        width = vintWidth(buffer[0]);

        if (width == 0) {
            throw ebmlInvalidVint("unpackVint(ioBase*, off_t, unsigned char&): Invalid start byte for vint.", DECODE_ERR_DEFAULT, offset);
        }

        if (width > 1) {
            readsize = file.read(buffer + 1, offset + 1, width - 1);

            if (readsize + 1 < width) {
                throw ebmlInvalidVint("Unexpected end of data while attempting to read vint.", DECODE_ERR_DEFAULT, offset);
            }
        }

        vintw = width;

        try {
            return unpackVint(buffer, width);
        } catch (ebmlDecodeError& e) {
            e.add_to_offset(offset);
            throw e;
        }
    }
}
#endif
