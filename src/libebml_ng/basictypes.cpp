#ifndef EBML_NG_BASICTYPES_CPP
#define EBML_NG_BASICTYPES_CPP

#include "basictypes.h"
#include "struct/ull.h"
#include "struct/ll.h"
#include "struct/double.h"
#include "struct/binary.h"
#include "struct/unicode.h"
#include "struct/datetime.h"
#include "struct.tpp"
#include "ebmlDataElement.tpp"
#include "masterelement/ebmlPair.tpp"
#include "masterelement/ebmlMap.tpp"

#include "ebmlElementType.tpp"
#include "ebmlElement.tpp"

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
