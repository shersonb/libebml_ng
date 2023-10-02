#ifndef EBML_NG_MASTERELEMENT_CLASS_CPP
#define EBML_NG_MASTERELEMENT_CLASS_CPP

#include "libebml_ng/masterelement/base.h"

namespace ebml {
    ebmlMasterElementClass::ebmlMasterElementClass(const char* ebmlID, const std::wstring& name) : ebmlMasterElementClass(unpackVint(ebmlID), name) {}
    ebmlMasterElementClass::ebmlMasterElementClass(ebmlID_t ebmlID, const std::wstring& name) : ebmlElementClass(ebmlID, name) {}
}
#endif
