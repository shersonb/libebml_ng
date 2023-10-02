#ifndef EBML_NG_HEAD_H
#define EBML_NG_HEAD_H

#include "basictypes.h"
#include "masterelement/multislot.h"

namespace ebml {
    extern ebmlUnsignedIntegerClass EBMLVersion;
    extern ebmlUnsignedIntegerClass EBMLReadVersion;
    extern ebmlUnsignedIntegerClass EBMLMaxIDLength;
    extern ebmlUnsignedIntegerClass EBMLMaxSizeLength;
    extern ebmlBinaryClass DocType;
    extern ebmlUnsignedIntegerClass DocTypeVersion;
    extern ebmlUnsignedIntegerClass DocTypeReadVersion;
    extern ebmlMultiSlotClass EBMLHead;
}

#endif
