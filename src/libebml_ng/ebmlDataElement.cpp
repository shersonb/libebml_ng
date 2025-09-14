#ifndef EBML_NG_DATAELEMENT_CPP
#define EBML_NG_DATAELEMENT_CPP

#include "ebmlDataElement.h"
#include "ebmlDataElement.tpp"
#include "ebmlElementType.tpp"
#include "ebmlElement.tpp"
#include "struct/ull.h"
#include "struct/ll.h"
#include "struct/double.h"
#include "struct/binary.h"
#include "struct/unicode.h"
#include "struct/datetime.h"
#include "struct.tpp"

INST_EBML_CLS(unsigned long long, 0);
INST_EBML_CLS(long long, 0);
INST_EBML_CLS(double, 0.0);
INST_EBML_CLS(std::string, "");
INST_EBML_CLS(std::wstring, L"");
INST_EBML_CLS(ebml::timepoint_t, ebml::epoch);

INST_CONST_EBML_CLS(unsigned long long, 0);
INST_CONST_EBML_CLS(long long, 0);
INST_CONST_EBML_CLS(double, 0.0);
INST_CONST_EBML_CLS(std::string, "");
INST_CONST_EBML_CLS(std::wstring, L"");
INST_CONST_EBML_CLS(ebml::timepoint_t, ebml::epoch);
#endif
