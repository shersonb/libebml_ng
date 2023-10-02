#ifndef EBML_NG_BASICTYPES_H
#define EBML_NG_BASICTYPES_H

#include "libebml_ng/template.h"
#include "libebml_ng/struct/datetime.h"

namespace ebml {
    extern template class ebmlDataElementClass<unsigned long long>;
    extern template class ebmlDataElementClass<long long>;
    extern template class ebmlDataElementClass<double>;
    extern template class ebmlDataElementClass<std::string>;
    extern template class ebmlDataElementClass<std::wstring>;
    extern template class ebmlDataElementClass<timepoint_t>;
    // extern template class ebmlMapClass<unsigned long long>;
    // extern template class ebmlMapClass<long long>;

    extern template class ebmlDataElement<unsigned long long>;
    extern template class ebmlDataElement<long long>;
    extern template class ebmlDataElement<double>;
    extern template class ebmlDataElement<std::string>;
    extern template class ebmlDataElement<std::wstring>;
    extern template class ebmlDataElement<timepoint_t>;
    // extern template class ebmlMap<unsigned long long>;
    // extern template class ebmlMap<long long>;

    typedef ebmlDataElementClass<unsigned long long> ebmlUnsignedIntegerClass;
    typedef ebmlDataElementClass<long long> ebmlSignedIntegerClass;
    typedef ebmlDataElementClass<double> ebmlFloatClass;
    typedef ebmlDataElementClass<std::string> ebmlBinaryClass;
    typedef ebmlDataElementClass<std::wstring> ebmlUnicodeClass;
    typedef ebmlDataElementClass<timepoint_t> ebmlDateTimeClass;

    typedef ebmlDataElement<unsigned long long> ebmlUnsignedInteger;
    typedef ebmlDataElement<long long> ebmlSignedInteger;
    typedef ebmlDataElement<double> ebmlFloat;
    typedef ebmlDataElement<std::string> ebmlBinary;
    typedef ebmlDataElement<std::wstring> ebmlUnicode;
    typedef ebmlDataElement<timepoint_t> ebmlDateTime;

    extern template unsigned long long& data<unsigned long long>(const ebmlElement_sp&);
    extern template const unsigned long long& data<unsigned long long>(const c_ebmlElement_sp&);

    extern template long long& data<long long>(const ebmlElement_sp&);
    extern template const long long& data<long long>(const c_ebmlElement_sp&);

    extern template double& data<double>(const ebmlElement_sp&);
    extern template const double& data<double>(const c_ebmlElement_sp&);

    extern template std::string& data<std::string>(const ebmlElement_sp&);
    extern template const std::string& data<std::string>(const c_ebmlElement_sp&);

    extern template std::wstring& data<std::wstring>(const ebmlElement_sp&);
    extern template const std::wstring& data<std::wstring>(const c_ebmlElement_sp&);

    extern template timepoint_t& data<timepoint_t>(const ebmlElement_sp&);
    extern template const timepoint_t& data<timepoint_t>(const c_ebmlElement_sp&);

}

#endif
