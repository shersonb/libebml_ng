#ifndef EBML_NG_DATAELEMENT_CPP
#define EBML_NG_DATAELEMENT_CPP

#include "libebml_ng/ebmlDataElement.h"
#include "libebml_ng/ebmlDataElement.tpp"
#include "libebml_ng/ebmlElementClass.tpp"
#include "libebml_ng/ebmlElement.tpp"

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
