// #include "libebml_ng/libebml_ng-exceptions.cpp"
#define buffersize 65536
#include "config.h"

#include "libebml_ng/masterelement/map.tpp"

namespace ebml {
    // template class ebmlMapClass<unsigned long long>;
    // template class ebmlMapClass<long long>;
    // template class ebmlMap<unsigned long long>;
    // template class ebmlMap<long long>;
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
