// #include "libebml_ng/libebml_ng-exceptions.cpp"
#define buffersize 65536
#include "config.h"
#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io/fd.cpp"
#include "libebml_ng/io/stdio.cpp"
#include "libebml_ng/io.tpp"
// #include "libebml_ng/struct/double.h"
#include "libebml_ng/base.h"
#include "libebml_ng/struct.h"
#include "libebml_ng/struct/ull.cpp"
#include "libebml_ng/struct/ll.cpp"
#include "libebml_ng/struct/double.cpp"
#include "libebml_ng/struct/binary.cpp"
#include "libebml_ng/struct/unicode.cpp"
#include "libebml_ng/struct.tpp"
#include "libebml_ng/template.h"

#include "libebml_ng/masterelement/base.cpp"
#include "libebml_ng/masterelement/list.cpp"
// #include "libebml_ng/masterelement/multislot.cpp"

#include "libebml_ng/template.tpp"
#include "libebml_ng/struct.cpp"
#include "libebml_ng/vint.cpp"
#include "libebml_ng/parsing/string.cpp"
#include "libebml_ng/parsing/io.cpp"
#include "libebml_ng/base.cpp"
#include "libebml_ng/repr.cpp"

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

    template class ebmlDataElementClass<unsigned long long>;
    template class ebmlDataElementClass<long long>;
    template class ebmlDataElementClass<double>;
    template class ebmlDataElementClass<std::string>;
    template class ebmlDataElementClass<std::wstring>;

    template class ebmlDataElement<unsigned long long>;
    template class ebmlDataElement<long long>;
    template class ebmlDataElement<double>;
    template class ebmlDataElement<std::string>;
    template class ebmlDataElement<std::wstring>;

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

#ifdef ENABLE_GZIP_SUPPORT
#if ENABLE_GZIP_SUPPORT
#pragma message("Gzip enabled")
#else
#pragma message("Gzip disabled")
#endif
#else
#pragma message("Gzip disabled")
#endif

// #include "libebml_ng/libebml_ng-base.cpp"
// #include "libebml_ng/libebml_ng-ull.cpp"
// #include "libebml_ng/libebml_ng-ll.cpp"
// #include "libebml_ng/libebml_ng-double.cpp"
// #include "libebml_ng/libebml_ng-binary.cpp"
// #include "libebml_ng/libebml_ng-unicode.cpp"
// #include "libebml_ng/libebml_ng-masterelement.cpp"
// #include "libebml_ng/libebml_ng-masterelement-list.cpp"
