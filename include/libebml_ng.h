#ifndef EBML_NG_H
#define EBML_NG_H

#include "libebml_ng/template.h"
#include "libebml_ng/io.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/base.h"
#include "libebml_ng/repr.h"
#include "libebml_ng/template.tpp"
#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/masterelement/list.h"
#include "libebml_ng/masterelement/multislot.h"

namespace ebml {
    extern template class ebmlDataElementClass<unsigned long long>;
    extern template class ebmlDataElementClass<long long>;
    extern template class ebmlDataElementClass<double>;
    extern template class ebmlDataElementClass<std::string>;
    extern template class ebmlDataElementClass<std::wstring>;

    extern template class ebmlDataElement<unsigned long long>;
    extern template class ebmlDataElement<long long>;
    extern template class ebmlDataElement<double>;
    extern template class ebmlDataElement<std::string>;
    extern template class ebmlDataElement<std::wstring>;

    typedef ebmlDataElementClass<unsigned long long> ebmlUnsignedIntegerClass;
    typedef ebmlDataElementClass<long long> ebmlSignedIntegerClass;
    typedef ebmlDataElementClass<double> ebmlFloatClass;
    typedef ebmlDataElementClass<std::string> ebmlBinaryClass;
    typedef ebmlDataElementClass<std::wstring> ebmlUnicodeClass;

    typedef ebmlDataElement<unsigned long long> ebmlUnsignedInteger;
    typedef ebmlDataElement<long long> ebmlSignedInteger;
    typedef ebmlDataElement<double> ebmlFloat;
    typedef ebmlDataElement<std::string> ebmlBinary;
    typedef ebmlDataElement<std::wstring> ebmlUnicode;
}

#endif
