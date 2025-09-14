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
    const size_t UNKNOWN = 0xffffffffffffffffULL;

    // Removed inline functions (packVint and unpackVint overloading with size parameter)
    // along with commented-out redundant versions that have now been defined in the header.

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
            // Use vintw = 0 to indicate end of stream.
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
