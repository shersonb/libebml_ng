#ifndef EBML_NG_MASTERELEMENT_PAIR_CPP
#define EBML_NG_MASTERELEMENT_PAIR_CPP

#include "libebml_ng/masterelement/ebmlPair.h"
#include "libebml_ng/struct/binary.h"
#include "libebml_ng/struct/ull.h"
#include "libebml_ng/masterelement/base/childSlot_t.tpp"
#include "libebml_ng/masterelement/ebmlList.h"
#include "libebml_ng/masterelement/ebmlPair.tpp"

namespace ebml {
    template class ebmlPairType<std::string, unsigned long long>;
    template class ebmlPair<std::string, unsigned long long>;
    // template class childSlot_t<ebmlList>;
    // template class ebmlPairClass<std::string, ebmlList>;
    // template class ebmlPair<std::string, ebmlList>;
}
#endif
