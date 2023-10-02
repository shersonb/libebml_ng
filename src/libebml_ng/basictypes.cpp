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

namespace ebml {
    template<>
    unsigned long long ebmlDataElementClass<unsigned long long>::defaultdefault = 0;

    template<>
    long long ebmlDataElementClass<long long>::defaultdefault = 0;

    template<>
    double ebmlDataElementClass<double>::defaultdefault = 0.0;

    template<>
    std::string ebmlDataElementClass<std::string>::defaultdefault = "";

    template<>
    std::wstring ebmlDataElementClass<std::wstring>::defaultdefault = L"";

    template<>
    timepoint_t ebmlDataElementClass<timepoint_t>::defaultdefault = epoch;

    template class ebmlDataElementClass<unsigned long long>;
    template class ebmlDataElementClass<long long>;
    template class ebmlDataElementClass<double>;
    template class ebmlDataElementClass<std::string>;
    template class ebmlDataElementClass<std::wstring>;
    template class ebmlDataElementClass<timepoint_t>;

    template class ebmlDataElement<unsigned long long>;
    template class ebmlDataElement<long long>;
    template class ebmlDataElement<double>;
    template class ebmlDataElement<std::string>;
    template class ebmlDataElement<std::wstring>;
    template class ebmlDataElement<timepoint_t>;

    template unsigned long long& data<unsigned long long>(const ebmlElement_sp&);
    template const unsigned long long& data<unsigned long long>(const c_ebmlElement_sp&);

    template long long& data<long long>(const ebmlElement_sp&);
    template const long long& data<long long>(const c_ebmlElement_sp&);

    template double& data<double>(const ebmlElement_sp&);
    template const double& data<double>(const c_ebmlElement_sp&);

    template std::string& data<std::string>(const ebmlElement_sp&);
    template const std::string& data<std::string>(const c_ebmlElement_sp&);

    template std::wstring& data<std::wstring>(const ebmlElement_sp&);
    template const std::wstring& data<std::wstring>(const c_ebmlElement_sp&);

    template timepoint_t& data<timepoint_t>(const ebmlElement_sp&);
    template const timepoint_t& data<timepoint_t>(const c_ebmlElement_sp&);
}

#endif
