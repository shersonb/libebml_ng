#ifndef EBML_NG_BASICTYPES_CPP
#define EBML_NG_BASICTYPES_CPP

#include "libebml_ng/basictypes.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/struct/datetime.h"
#include "libebml_ng/template.tpp"

INST_EBML_CLS(unsigned long long, 0)
INST_EBML_CLS(long long, 0)
INST_EBML_CLS(double, 0.0)
INST_EBML_CLS(std::string, "")
INST_EBML_CLS(std::wstring, L"")
INST_EBML_CLS(ebml::timepoint_t, ebml::epoch)

INST_CONST_EBML_CLS(unsigned long long, 0)
INST_CONST_EBML_CLS(long long, 0)
INST_CONST_EBML_CLS(double, 0.0)
INST_CONST_EBML_CLS(std::string, "")
INST_CONST_EBML_CLS(std::wstring, L"")
INST_CONST_EBML_CLS(ebml::timepoint_t, ebml::epoch)

#endif
