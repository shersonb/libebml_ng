#ifndef EBML_NG_VINT_H
#define EBML_NG_VINT_H

#include "libebml_ng/io.h"

namespace ebml {
    // Detect length of encoded vint based on first character.
    vintWidth_t vintWidth(char);

    // Detect minimum width required to encode unsigned long long as a vint.
    vintWidth_t widthAsVint(unsigned long long);

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
    unsigned long long unpackVint(ioBase*, vintWidth_t&);

    // Same as above, but allows for an offset to be specified.
    unsigned long long unpackVint(ioBase*, off_t, vintWidth_t&);
}
#endif
