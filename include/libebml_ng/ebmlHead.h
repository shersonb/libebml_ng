#ifndef EBML_NG_HEAD_H
#define EBML_NG_HEAD_H

#include "basictypes.h"
#include "masterelement/ebmlMultiSlot.h"

namespace ebml {
    extern const ebmlUnsignedIntegerClass EBMLVersion;
    extern const ebmlUnsignedIntegerClass EBMLReadVersion;
    extern const ebmlUnsignedIntegerClass EBMLMaxIDLength;
    extern const ebmlUnsignedIntegerClass EBMLMaxSizeLength;
    extern const ebmlBinaryClass DocType;
    extern const ebmlUnsignedIntegerClass DocTypeVersion;
    extern const ebmlUnsignedIntegerClass DocTypeReadVersion;

    extern const ebmlMultiSlotClass EBMLHead;
}

#endif
