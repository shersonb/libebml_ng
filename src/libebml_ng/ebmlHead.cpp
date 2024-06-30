#ifndef EBML_NG_HEAD_CPP
#define EBML_NG_HEAD_CPP

#include "libebml_ng/ebmlHead.h"

namespace ebml {
    const ebmlUnsignedIntegerClass EBMLVersion = ebmlUnsignedIntegerClass("\x42\x86", L"EBMLVersion", 1);
    const ebmlUnsignedIntegerClass EBMLReadVersion = ebmlUnsignedIntegerClass("\x42\xf7", L"EBMLReadVersion", 1);
    const ebmlUnsignedIntegerClass EBMLMaxIDLength = ebmlUnsignedIntegerClass("\x42\xf2", L"EBMLMaxIDLength", 4);
    const ebmlUnsignedIntegerClass EBMLMaxSizeLength = ebmlUnsignedIntegerClass("\x42\xf3", L"EBMLMaxSizeLength", 8);
    const ebmlBinaryClass DocType = ebmlBinaryClass("\x42\x82", L"DocType");
    const ebmlUnsignedIntegerClass DocTypeVersion = ebmlUnsignedIntegerClass("\x42\x87", L"DocTypeVersion", 1);
    const ebmlUnsignedIntegerClass DocTypeReadVersion = ebmlUnsignedIntegerClass("\x42\x85", L"DocTypeReadVersion", 1);

    const ebmlMultiSlotClass EBMLHead = ebmlMultiSlotClass("\x1a\x45\xdf\xa3", L"EBMLHead", {
        {"ebmlVersion", {{&EBMLVersion, 1, 1}}},
        {"ebmlReadVersion", {{&EBMLReadVersion, 1, 1}}},
        {"ebmlMaxIDLength", {{&EBMLMaxIDLength, 1, 1}}},
        {"ebmlMaxSizeLength", {{&EBMLMaxSizeLength, 1, 1}}},
        {"docType", {{&DocType, 1, 1}}},
        {"docTypeVersion", {{&DocTypeVersion, 1, 1}}},
        {"docTypeReadVersion", {{&DocTypeReadVersion, 1, 1}}}
    });
}

#endif
