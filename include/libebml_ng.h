#ifndef EBML_NG_H
#define EBML_NG_H

#include "libebml_ng/struct/ull.h"
#include "libebml_ng/struct/ll.h"
#include "libebml_ng/struct/double.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/base.h"
#include "libebml_ng/template.h"
#include "libebml_ng/template.tpp"
// #include "libebml_ng/struct/unicode.h"
// #include "libebml_ng/masterelement.h"
// #include "libebml_ng-masterelement-list.h"

namespace ebml {
    extern template class ebmlElementClass<unsigned long long>;
    extern template class ebmlElementClass<long long>;
    extern template class ebmlElementClass<double>;
    extern template class ebmlElementClass<std::string>;

    extern template class ebmlElement<unsigned long long>;
    extern template class ebmlElement<long long>;
    extern template class ebmlElement<double>;
    extern template class ebmlElement<std::string>;

    typedef ebmlElementClass<unsigned long long> ebmlUnsignedIntegerClass;
    typedef ebmlElementClass<long long> ebmlSignedIntegerClass;
    typedef ebmlElementClass<double> ebmlFloatClass;
    typedef ebmlElementClass<std::string> ebmlBinaryClass;

    typedef ebmlElement<unsigned long long> ebmlUnsignedInteger;
    typedef ebmlElement<long long> ebmlSignedInteger;
    typedef ebmlElement<double> ebmlFloat;
    typedef ebmlElement<std::string> ebmlBinary;
}

#endif
