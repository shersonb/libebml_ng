#ifndef EBML_NG_LAZYLOAD_PREPARED_RESIZE_CPP
#define EBML_NG_LAZYLOAD_PREPARED_RESIZE_CPP

#include "libebml_ng/masterelement/lazyload/prepared_resize_t.h"
#include "libebml_ng/exceptions.h"

#include "libebml_ng/masterelement/lazyload/status.tpp"

namespace ebml {
    prepared_resize_t::prepared_resize_t()
    : endOffset(0), nextSibling(0), seekData(nullptr) {}

    prepared_resize_t::prepared_resize_t(const extent_t& lastChild, off_t endOffset, off_t next) : lastChild(lastChild), endOffset(endOffset), nextSibling(next), seekData(nullptr) {}

    void prepared_resize_t::throw_exc(const std::string& msg, const c_ebmlElement_sp&) const {
        throw ebmlWriteError(msg);
    }

    template class status_t<prepared_resize_t>;
}
#endif

