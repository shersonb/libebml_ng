#ifndef EBML_NG_VINT_CPP
#define EBML_NG_VINT_CPP

#include <string.h>
#include <stdio.h>
#include <array>
#include "libebml_ng/exceptions.cpp"
#include "libebml_ng/vint.h"
#include "libebml_ng/struct/ull.cpp"

namespace ebml {
    unsigned char vintWidth(char b) {
        static const std::array<unsigned char, 8> masks = {{
            0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
        }};
        unsigned char k;

        for (k = 0; k < 8; ++k) {
            if (b & masks[k]) {
                return k + 1;
            }
        }

        throw std::invalid_argument("Variable-length integer cannot start with null byte.");
    }

    unsigned char widthAsVint(unsigned long long n) {
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

    void packVint(unsigned long long n, unsigned char size, char* dest) {
        if (size == 0 or size > 8) {
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

    unsigned char packVint(unsigned long long n, char* dest) {
        unsigned char size = widthAsVint(n);
        packVint(n, size, dest);
        return size;
    }

    unsigned long long unpackVint(const char* data, unsigned char size) {
        unsigned long long val;

        val = unpack<unsigned long long>(data, size);
        val &= ~(1ULL << (7*size));

        if (val == (1ULL << (7*size)) - 1ULL) {
            return 0xffffffffffffffff;
        };

        return val;
    }

    unsigned long long unpackVint(const char* data) {
        unsigned char size;

        size = vintWidth(data[0]);
        return unpackVint(data, size);
    }

    unsigned long long unpackVint(const char* data, size_t dataSize, unsigned char* vintW) {
        unsigned char size;

        if (dataSize == 0) {
            throw ebmlException("No data", __LINE__, __FILE__);
        }

        size = vintWidth(data[0]);

        if (dataSize < size) {
            throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
        }

        vintW[0] = size;
        return unpackVint(data, size);
    }

//     unsigned long long unpackVint(FILE* file, char* buffer, unsigned char* vintw) {
//         unsigned char width;
//         unsigned char readsize;
// 
//         if (fread(buffer, 1, 1, file) == 0) {
//             /*Use vint[0] = 0 to indicate end of stream.*/
//             vintw[0] = 0;
//             return 0;
//         }
// 
//         width = vintWidth(buffer[0]);
// 
//         if (width == 0) {
//             throw ebmlException("Invalid start byte for vint.", __LINE__, __FILE__);
//         }
// 
//         if (width > 1) {
//             readsize = fread(buffer + 1, width - 1, 1, file);
// 
//             if (readsize + 1 < width) {
//                 throw ebmlException("Unexpected end of data while attempting to read vint.", __LINE__, __FILE__);
//             }
//         }
//         vintw[0] = width;
//         return unpackVint(buffer, width);
//     }
// 
//     unsigned long long unpackVint(
//             FILE* file, unsigned char* vintw) {
//         char buffer[16];
//         return unpackVint(file, &buffer[0], vintw);
//     }
    unsigned long long unpackVint(ioBase* file, char* buffer, unsigned char* vintw) {
        unsigned char width;
        unsigned char readsize;

        if (file->read(buffer, 1) == 0) {
            /*Use vint[0] = 0 to indicate end of stream.*/
            vintw[0] = 0;
            return 0;
        }

        width = vintWidth(buffer[0]);

        if (width == 0) {
            throw ebmlException("Invalid start byte for vint.", __LINE__, __FILE__);
        }

        if (width > 1) {
            readsize = file->read(buffer + 1, width - 1);

            if (readsize + 1 < width) {
                throw ebmlException("Unexpected end of data while attempting to read vint.", __LINE__, __FILE__);
            }
        }
        vintw[0] = width;
        return unpackVint(buffer, width);
    }

    unsigned long long unpackVint(
            ioBase* file, unsigned char* vintw) {
        char buffer[8];
        return unpackVint(file, &buffer[0], vintw);
    }

    unsigned long long unpackVint(ioBase* file, char* buffer, off_t offset, unsigned char* vintw) {
        unsigned char width;
        unsigned char readsize;

        if (file->read(buffer, 1, offset) == 0) {
            /*Use vint[0] = 0 to indicate end of stream.*/
            vintw[0] = 0;
            return 0;
        }

        width = vintWidth(buffer[0]);

        if (width == 0) {
            throw ebmlException("Invalid start byte for vint.", __LINE__, __FILE__);
        }

        if (width > 1) {
            readsize = file->read(buffer + 1, width - 1, offset + 1);

            if (readsize + 1 < width) {
                throw ebmlException("Unexpected end of data while attempting to read vint.", __LINE__, __FILE__);
            }
        }
        vintw[0] = width;
        return unpackVint(buffer, width);
    }

    unsigned long long unpackVint(
            ioBase* file, off_t offset, unsigned char* vintw) {
        char buffer[8];
        return unpackVint(file, &buffer[0], offset, vintw);
    }

}
#endif
