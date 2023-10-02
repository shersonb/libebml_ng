#ifndef EBML_NG_BASICTYPES_H
#define EBML_NG_BASICTYPES_H

#include "libebml_ng/template.h"
#include "libebml_ng/struct/datetime.h"

DECL_EBML_CLS(unsigned long long)
DECL_EBML_CLS(long long)
DECL_EBML_CLS(double)
DECL_EBML_CLS(std::string)
DECL_EBML_CLS(std::wstring)
DECL_EBML_CLS(ebml::timepoint_t)

namespace ebml {
    DECL_EBML_ALIAS(unsigned long long, ebmlUnsignedInteger)
    DECL_EBML_ALIAS(long long, ebmlSignedInteger)
    DECL_EBML_ALIAS(double, ebmlFloat)
    DECL_EBML_ALIAS(std::string, ebmlBinary)
    DECL_EBML_ALIAS(std::wstring, ebmlUnicode)
    DECL_EBML_ALIAS(ebml::timepoint_t, ebmlDateTime)
}

#endif
