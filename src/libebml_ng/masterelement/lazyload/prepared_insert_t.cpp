#ifndef EBML_NG_LAZYLOAD_PREPARED_INSERT_CPP
#define EBML_NG_LAZYLOAD_PREPARED_INSERT_CPP

#include "libebml_ng/masterelement/lazyload/prepared_insert_t.h"
#include "libebml_ng/exceptions.h"

#include "libebml_ng/masterelement/lazyload/status.tpp"

namespace ebml {
    prepared_insert_t::prepared_insert_t()
    : prev(0), extent(0), next(0) {}

    prepared_insert_t::prepared_insert_t(const extent_t& prev, const extent_t& extent, off_t next) : prev(prev), extent(extent), next(next) {}
    prepared_insert_t::prepared_insert_t(const extent_t& prev, extent_t&& extent, off_t next) : prev(prev), extent(std::move(extent)), next(next) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, const extent_t& extent, off_t next) : prev(std::move(prev)), extent(extent), next(next) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, extent_t&& extent, off_t next) : prev(std::move(prev)), extent(std::move(extent)), next(next) {}

    prepared_insert_t::prepared_insert_t(const extent_t& prev, const extent_t& extent, off_t next, const children_m::iterator& hint) : prev(prev), extent(extent), next(next), hint(hint) {}
    prepared_insert_t::prepared_insert_t(const extent_t& prev, extent_t&& extent, off_t next, const children_m::iterator& hint) : prev(prev), extent(std::move(extent)), next(next), hint(hint) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, const extent_t& extent, off_t next, const children_m::iterator& hint) : prev(std::move(prev)), extent(extent), next(next), hint(hint) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, extent_t&& extent, off_t next, const children_m::iterator& hint) : prev(std::move(prev)), extent(std::move(extent)), next(next), hint(hint) {}

    prepared_insert_t::prepared_insert_t(const extent_t& prev, const extent_t& extent, off_t next, children_m::iterator&& hint) : prev(prev), extent(extent), next(next), hint(std::move(hint)) {}
    prepared_insert_t::prepared_insert_t(const extent_t& prev, extent_t&& extent, off_t next, children_m::iterator&& hint) : prev(prev), extent(std::move(extent)), next(next), hint(std::move(hint)) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, const extent_t& extent, off_t next, children_m::iterator&& hint) : prev(std::move(prev)), extent(extent), next(next), hint(std::move(hint)) {}
    prepared_insert_t::prepared_insert_t(extent_t&& prev, extent_t&& extent, off_t next, children_m::iterator&& hint) : prev(std::move(prev)), extent(std::move(extent)), next(next), hint(std::move(hint)) {}

    void prepared_insert_t::throw_exc(const std::string& msg, const c_ebmlElement_sp& elem) const {
        throw ebmlInsertionError(msg, prev.offset, prev.endOffset, extent.offset, extent.endOffset, next, elem);
    }

    template class status_t<prepared_insert_t>;
}
#endif

