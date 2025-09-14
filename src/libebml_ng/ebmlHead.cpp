#ifndef EBML_NG_HEAD_CPP
#define EBML_NG_HEAD_CPP

#include "ebmlHead.h"

namespace ebml {
    const ebmlUnsignedIntegerType EBMLVersion = ebmlUnsignedIntegerType("\x42\x86", L"EBMLVersion", 1);
    const ebmlUnsignedIntegerType EBMLReadVersion = ebmlUnsignedIntegerType("\x42\xf7", L"EBMLReadVersion", 1);
    const ebmlUnsignedIntegerType EBMLMaxIDLength = ebmlUnsignedIntegerType("\x42\xf2", L"EBMLMaxIDLength", 4);
    const ebmlUnsignedIntegerType EBMLMaxSizeLength = ebmlUnsignedIntegerType("\x42\xf3", L"EBMLMaxSizeLength", 8);
    const ebmlBinaryType DocType = ebmlBinaryType("\x42\x82", L"DocType");
    const ebmlUnsignedIntegerType DocTypeVersion = ebmlUnsignedIntegerType("\x42\x87", L"DocTypeVersion", 1);
    const ebmlUnsignedIntegerType DocTypeReadVersion = ebmlUnsignedIntegerType("\x42\x85", L"DocTypeReadVersion", 1);

    const ebmlStructType EBMLHead = ebmlStructType("\x1a\x45\xdf\xa3", L"EBMLHead", {
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
