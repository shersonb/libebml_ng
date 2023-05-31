// #include "libebml_ng/libebml_ng-exceptions.cpp"
#include "config.h"
#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/io/fd.cpp"
#include "libebml_ng/io/stdio.cpp"
// #include "libebml_ng/struct/double.h"
#include "libebml_ng/base.h"
#include "libebml_ng/struct.h"
#include "libebml_ng/struct/ull.cpp"
#include "libebml_ng/struct/ll.cpp"
#include "libebml_ng/struct/double.cpp"
#include "libebml_ng/struct/binary.cpp"
#include "libebml_ng/template.h"
#include "libebml_ng/struct/unicode.cpp"
#include "libebml_ng/template.tpp"
#include "libebml_ng/struct.cpp"
#include "libebml_ng/vint.cpp"
#include "libebml_ng/parsing/string.cpp"
#include "libebml_ng/parsing/io.cpp"
#include "libebml_ng/base.cpp"

namespace ebml {
    template class ebmlElementClass<unsigned long long>;
    template class ebmlElementClass<long long>;
    template class ebmlElementClass<double>;
    template class ebmlElementClass<std::string>;
    template class ebmlElementClass<unicode>;

    template class ebmlElement<unsigned long long>;
    template class ebmlElement<long long>;
    template class ebmlElement<double>;
    template class ebmlElement<std::string>;
    template class ebmlElement<unicode>;

    typedef ebmlElementClass<unsigned long long> ebmlUnsignedIntegerClass;
    typedef ebmlElementClass<long long> ebmlSignedIntegerClass;
    typedef ebmlElementClass<double> ebmlFloatClass;
    typedef ebmlElementClass<std::string> ebmlBinaryClass;
    typedef ebmlElementClass<unicode> ebmlUnicodeClass;

    typedef ebmlElement<unsigned long long> ebmlUnsignedInteger;
    typedef ebmlElement<long long> ebmlSignedInteger;
    typedef ebmlElement<double> ebmlFloat;
    typedef ebmlElement<std::string> ebmlBinary;
    typedef ebmlElement<unicode> ebmlUnicode;
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
