#ifndef EBML_NG_LAZYLOAD_HELPER_CPP
#define EBML_NG_LAZYLOAD_HELPER_CPP

#include "libebml_ng/masterelement/lazyload/childElemHelper_t.h"
// #include "ebmlLazyLoad.h"
// #include <unordered_map>
// #include <map>
// #include <unordered_set>
// #include <mutex>
// #include <shared_mutex>
//
// #include "base.h"
// #include "../seekdata.h"
// #include "../exceptions.h"
// #include "../ebmlDataElement.h"


namespace ebml {
    childElemHelper_t::childElemHelper_t() {}
    void childElemHelper_t::_onBeforeInsert(ebmlLazyLoad&, off_t, const ebmlElement_sp&) const {}
    void childElemHelper_t::_onAfterInsert(ebmlLazyLoad&, off_t, const ebmlElement_sp&, seekData_t*) const {}
    void childElemHelper_t::_onBeforeMove(ebmlLazyLoad&) const {}
    void childElemHelper_t::_onAfterMove(ebmlLazyLoad&) const {}
    void childElemHelper_t::_onBeforeRemove(ebmlLazyLoad&) const {}
    void childElemHelper_t::_onAfterRemove(ebmlLazyLoad&) const {}

    childElemHelper_t childHelper;

}

#endif
