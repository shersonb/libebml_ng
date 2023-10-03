#ifndef EBML_NG_BASICTYPES_H
#define EBML_NG_BASICTYPES_H

#include "libebml_ng/template.h"
#include "libebml_ng/struct/datetime.h"

#define DECL_EBML_CLS(T) \
    extern template class ebml::ebmlDataElementClass<T>; \
    extern template class ebml::ebmlDataElement<T>; \
    extern template T& ebml::data<T>(const ebmlElement_sp&); \
    extern template const T& ebml::data<T>(const c_ebmlElement_sp&);


DECL_EBML_CLS(unsigned long long)
DECL_EBML_CLS(long long)
DECL_EBML_CLS(double)
DECL_EBML_CLS(std::string)
DECL_EBML_CLS(std::wstring)
DECL_EBML_CLS(ebml::timepoint_t)

#define DECL_EBML_ALIAS(T, alias) \
    typedef ::ebml::ebmlDataElementClass<T> alias##Class; \
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
