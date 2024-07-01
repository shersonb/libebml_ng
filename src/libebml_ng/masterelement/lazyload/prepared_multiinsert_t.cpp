#ifndef EBML_NG_LAZYLOAD_PREPARED_MULTIINSERT_CPP
#define EBML_NG_LAZYLOAD_PREPARED_MULTIINSERT_CPP

#include "libebml_ng/masterelement/lazyload/prepared_multiinsert_t.h"
#include "libebml_ng/exceptions.h"

#include "libebml_ng/masterelement/lazyload/status.tpp"

namespace ebml {
    prepared_multiinsert_t::prepared_multiinsert_t()
    : prev(0), next(0) {}

    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, const std::vector<extent_t>& extents, off_t next) : prev(prev), extents(extents), next(next) {}
    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, std::vector<extent_t>&& extents, off_t next) : prev(prev), extents(std::move(extents)), next(next) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, const std::vector<extent_t>& extents, off_t next) : prev(std::move(prev)), extents(extents), next(next) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, std::vector<extent_t>&& extents, off_t next) : prev(std::move(prev)), extents(std::move(extents)), next(next) {}

    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, const std::vector<extent_t>& extents, off_t next, const children_m::iterator& hint) : prev(prev), extents(extents), next(next), hint(hint) {}
    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, std::vector<extent_t>&& extents, off_t next, const children_m::iterator& hint) : prev(prev), extents(std::move(extents)), next(next), hint(hint) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, const std::vector<extent_t>& extents, off_t next, const children_m::iterator& hint) : prev(std::move(prev)), extents(extents), next(next), hint(hint) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, std::vector<extent_t>&& extents, off_t next, const children_m::iterator& hint) : prev(std::move(prev)), extents(std::move(extents)), next(next), hint(hint) {}

    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, const std::vector<extent_t>& extents, off_t next, children_m::iterator&& hint) : prev(prev), extents(extents), next(next), hint(std::move(hint)) {}
    prepared_multiinsert_t::prepared_multiinsert_t(const extent_t& prev, std::vector<extent_t>&& extents, off_t next, children_m::iterator&& hint) : prev(prev), extents(std::move(extents)), next(next), hint(std::move(hint)) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, const std::vector<extent_t>& extents, off_t next, children_m::iterator&& hint) : prev(std::move(prev)), extents(extents), next(next), hint(std::move(hint)) {}
    prepared_multiinsert_t::prepared_multiinsert_t(extent_t&& prev, std::vector<extent_t>&& extents, off_t next, children_m::iterator&& hint) : prev(std::move(prev)), extents(std::move(extents)), next(next), hint(std::move(hint)) {}

    void prepared_multiinsert_t::throw_exc(const std::string& msg, const c_ebmlElement_sp& elem) const {
        throw ebmlInsertionError(msg, prev.offset, prev.endOffset, extents.front().offset, extents.back().endOffset, next, elem);
    }

    template class status_t<prepared_multiinsert_t>;
}
#endif

