// #include "libebml_ng/libebml_ng-exceptions.cpp"
#define buffersize 65536
#include "config.h"

// #include "libebml_ng/struct/ull.h"
// #include "libebml_ng/struct/ll.h"
// #include "libebml_ng/struct/double.h"
// #include "libebml_ng/struct/binary.h"
// #include "libebml_ng/struct/unicode.h"
// #include "libebml_ng/template.tpp"
#include "libebml_ng/masterelement/map.tpp"

namespace ebml {
    // template<>
    // unsigned long long ebmlDataElementClass<unsigned long long>::defaultdefault = 0;
    //
    // template<>
    // long long ebmlDataElementClass<long long>::defaultdefault = 0;
    //
    // template<>
    // double ebmlDataElementClass<double>::defaultdefault = 0.0;
    //
    // template<>
    // std::string ebmlDataElementClass<std::string>::defaultdefault = "";
    //
    // template<>
    // std::wstring ebmlDataElementClass<std::wstring>::defaultdefault = L"";

    // template class ebmlDataElementClass<unsigned long long>;
    // template class ebmlDataElementClass<long long>;
    // template class ebmlDataElementClass<double>;
    // template class ebmlDataElementClass<std::string>;
    // template class ebmlDataElementClass<std::wstring>;
    template class ebmlMapClass<unsigned long long>;
    template class ebmlMapClass<long long>;

    // template class ebmlDataElement<unsigned long long>;
    // template class ebmlDataElement<long long>;
    // template class ebmlDataElement<double>;
    // template class ebmlDataElement<std::string>;
    // template class ebmlDataElement<std::wstring>;
    template class ebmlMap<unsigned long long>;
    template class ebmlMap<long long>;
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
