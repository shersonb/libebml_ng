#ifndef EBML_NG_MASTERELEMENT_C_EBMLELEMENT_L_CPP
#define EBML_NG_MASTERELEMENT_C_EBMLELEMENT_L_CPP

#include "libebml_ng/masterelement/base/c_ebmlElement_l.h"

namespace ebml {
    c_ebmlElement_l::c_ebmlElement_l(const ebmlElement_l& items) : _items(&items) {}
}
#endif
