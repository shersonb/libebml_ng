#ifndef EBML_NG_VINT_H
#define EBML_NG_VINT_H

#include "libebml_ng/io.h"

namespace ebml {
    // Detect length of encoded vint based on first character.
    unsigned char vintWidth(char);

    // Detect minimum width required to encode unsigned long long as a vint.
    unsigned char widthAsVint(unsigned long long);

    // Encode vint, auto-detect vint-width
    unsigned char packVint(unsigned long long, char*);

    // Encode vint with provided vint-width.
    void packVint(unsigned long long, unsigned char, char*);

    // Decode vint using known vint-width obtained from vintWidht.
    // Warning! No validation is performed! Use with caution!
    unsigned long long unpackVint(const char*, unsigned char size);

    // Decode vint from char array.
    unsigned long long unpackVint(const char*);

    // Decode vint from char array, providing the size of char array.
    // Raises exception on unexpected end of data.
    // Stores vint-width in provided unsigned char array.
    unsigned long long unpackVint(const char*, size_t, unsigned char*);

    // Decode vint from file.
    // Stores vint-width in provided unsigned char array.
    unsigned long long unpackVint(ioBase*, unsigned char*);

    // Decode vint from file, using provided char array to perform read operations.
    // Stores vint-width in provided unsigned char array.
    unsigned long long unpackVint(ioBase*, char*, unsigned char*);

    // Same as above two, but allows for an offset to be specified.
    unsigned long long unpackVint(ioBase*, off_t, unsigned char*);
    unsigned long long unpackVint(ioBase*, char*, off_t, unsigned char*);
}
#endif
