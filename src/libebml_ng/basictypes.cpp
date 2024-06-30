#ifndef EBML_NG_BASICTYPES_CPP
#define EBML_NG_BASICTYPES_CPP

#include "libebml_ng/basictypes.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/struct/datetime.h"
#include "libebml_ng/ebmlDataElement.tpp"
#include "libebml_ng/masterelement/ebmlPair.tpp"
#include "libebml_ng/masterelement/ebmlMap.tpp"

#include "libebml_ng/ebmlElementClass.tpp"
#include "libebml_ng/ebmlElement.tpp"

INST_DATAMAP_TYPE(unsigned long long, unsigned long long);
INST_DATAMAP_TYPE(std::string, unsigned long long);
INST_DATAMAP_TYPE(std::string, long long);
INST_DATAMAP_TYPE(std::wstring, unsigned long long);
INST_DATAMAP_TYPE(std::wstring, long long);
INST_DATAMAP_TYPE(unsigned long long, std::string);
INST_DATAMAP_TYPE(unsigned long long, std::wstring);
INST_DATAMAP_TYPE(double, double);
// INST_DATAMAP_TYPE(double, ebml::ebmlElement);

#endif
