#ifndef EBML_NG_HEAD_H
#define EBML_NG_HEAD_H

#include "basictypes.h"
#include "masterelement/ebmlStruct.h"

namespace ebml {
    extern const ebmlUnsignedIntegerType EBMLVersion;
    extern const ebmlUnsignedIntegerType EBMLReadVersion;
    extern const ebmlUnsignedIntegerType EBMLMaxIDLength;
    extern const ebmlUnsignedIntegerType EBMLMaxSizeLength;
    extern const ebmlBinaryType DocType;
    extern const ebmlUnsignedIntegerType DocTypeVersion;
    extern const ebmlUnsignedIntegerType DocTypeReadVersion;

    extern const ebmlStructType EBMLHead;
}

#endif
