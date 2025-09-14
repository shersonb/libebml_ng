#ifndef EBML_NG_BASICTYPES_H
#define EBML_NG_BASICTYPES_H

#include <string>

#include "ebmlElementType.h"
#include "ebmlElement.h"
#include "ebmlDataElement.h"
#include "struct/datetime.h"
#include "masterelement/ebmlMap.h"

// DECL_DATA_CLS(unsigned long long)
// DECL_DATA_CLS(long long)
// DECL_DATA_CLS(double)
// DECL_DATA_CLS(std::string)
// DECL_DATA_CLS(std::wstring)
// DECL_DATA_CLS(ebml::timepoint_t)

// EXTERN_MAP_TYPE(double)
// EXTERN_MAP_TYPE(long long)
// EXTERN_MAP_TYPE(unsigned long long)
// EXTERN_MAP_TYPE(std::string)
// EXTERN_MAP_TYPE(std::wstring)

EXTERN_DATAMAP_TYPE(unsigned long long, unsigned long long);
EXTERN_DATAMAP_TYPE(std::string, unsigned long long);
EXTERN_DATAMAP_TYPE(std::string, long long);
EXTERN_DATAMAP_TYPE(std::wstring, unsigned long long);
EXTERN_DATAMAP_TYPE(std::wstring, long long);
EXTERN_DATAMAP_TYPE(unsigned long long, std::string);
EXTERN_DATAMAP_TYPE(unsigned long long, std::wstring);
EXTERN_DATAMAP_TYPE(double, double);


#define DECL_EBML_ALIAS(T, alias) \
    typedef ::ebml::ebmlDataType<T> alias##Type; \
    typedef ::ebml::ebmlDataElement<T> alias;


namespace ebml {
    DECL_EBML_ALIAS(unsigned long long, ebmlUnsignedInteger)
    DECL_EBML_ALIAS(long long, ebmlSignedInteger)
    DECL_EBML_ALIAS(double, ebmlFloat)
    DECL_EBML_ALIAS(std::string, ebmlBinary)
    DECL_EBML_ALIAS(std::wstring, ebmlUnicode)
    DECL_EBML_ALIAS(timepoint_t, ebmlDateTime)
}

#endif
