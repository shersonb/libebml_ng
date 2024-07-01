#ifndef EBML_NG_LAZYLOAD_PREPARED_REMOVE_CPP
#define EBML_NG_LAZYLOAD_PREPARED_REMOVE_CPP

#include "libebml_ng/masterelement/lazyload/prepared_remove_t.h"
#include "libebml_ng/exceptions.h"

#include "libebml_ng/masterelement/lazyload/status.tpp"

    // class prepared_remove_t {
    // public:
    //     extent_t prev;
    //     off_t offset;
    //     off_t next;
    //     ebmlElement_sp elem;
    //     children_m::iterator hint;
    //
    //     prepared_remove_t();
    //
    //     prepared_remove_t(const extent_t&, off_t, off_t, const ebmlElement_sp&, const children_m::iterator&);
    //     prepared_remove_t(const extent_t&, off_t, off_t, const ebmlElement_sp&, children_m::iterator&&);
    //     prepared_remove_t(const extent_t&, off_t, off_t, ebmlElement_sp&&, const children_m::iterator&);
    //     prepared_remove_t(const extent_t&, off_t, off_t, ebmlElement_sp&&, children_m::iterator&&);
    //     prepared_remove_t(const extent_t&, off_t, off_t);
    //
    //     prepared_remove_t(extent_t&&, off_t, off_t, const ebmlElement_sp&, const children_m::iterator&);
    //     prepared_remove_t(extent_t&&, off_t, off_t, const ebmlElement_sp&, children_m::iterator&&);
    //     prepared_remove_t(extent_t&&, off_t, off_t, ebmlElement_sp&&, const children_m::iterator&);
    //     prepared_remove_t(extent_t&&, off_t, off_t, ebmlElement_sp&&, children_m::iterator&&);
    //     prepared_remove_t(extent_t&&, off_t, off_t);
    //
    //     prepared_remove_t(const prepared_remove_t&) = default;
    //     prepared_remove_t(prepared_remove_t&&) = default;
    //
    //     prepared_remove_t& operator=(const prepared_remove_t&) = default;
    //     prepared_remove_t& operator=(prepared_remove_t&&) = default;
    //
    //     void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
    //     // operator ebmlMoveError() const;
    // };
    //

namespace ebml {
    prepared_remove_t::prepared_remove_t()
    : prev(0), offset(0), next(0) {}

    prepared_remove_t::prepared_remove_t(const extent_t& prev, off_t offset, off_t next, const ebmlElement_sp& elem, const children_m::iterator& hint)
    : prev(prev), offset(offset), next(next), elem(elem), hint(hint) {}

    prepared_remove_t::prepared_remove_t(const extent_t& prev, off_t offset, off_t next, const ebmlElement_sp& elem, children_m::iterator&& hint)
    : prev(prev), offset(offset), next(next), elem(elem), hint(std::move(hint)) {}

    prepared_remove_t::prepared_remove_t(const extent_t& prev, off_t offset, off_t next, ebmlElement_sp&& elem, const children_m::iterator& hint)
    : prev(prev), offset(offset), next(next), elem(std::move(elem)), hint(hint) {}

    prepared_remove_t::prepared_remove_t(const extent_t& prev, off_t offset, off_t next, ebmlElement_sp&& elem, children_m::iterator&& hint)
    : prev(prev), offset(offset), next(next), elem(std::move(elem)), hint(std::move(hint)) {}

    prepared_remove_t::prepared_remove_t(const extent_t& prev, off_t offset, off_t next) : prev(prev), offset(offset), next(next) {}

    prepared_remove_t::prepared_remove_t(extent_t&& prev, off_t offset, off_t next, const ebmlElement_sp& elem, const children_m::iterator& hint)
    : prev(std::move(prev)), offset(offset), next(next), elem(elem), hint(hint) {}

    prepared_remove_t::prepared_remove_t(extent_t&& prev, off_t offset, off_t next, const ebmlElement_sp& elem, children_m::iterator&& hint)
    : prev(std::move(prev)), offset(offset), next(next), elem(elem), hint(std::move(hint)) {}

    prepared_remove_t::prepared_remove_t(extent_t&& prev, off_t offset, off_t next, ebmlElement_sp&& elem, const children_m::iterator& hint)
    : prev(std::move(prev)), offset(offset), next(next), elem(std::move(elem)), hint(hint) {}

    prepared_remove_t::prepared_remove_t(extent_t&& prev, off_t offset, off_t next, ebmlElement_sp&& elem, children_m::iterator&& hint)
    : prev(std::move(prev)), offset(offset), next(next), elem(std::move(elem)), hint(std::move(hint)) {}

    prepared_remove_t::prepared_remove_t(extent_t&& prev, off_t offset, off_t next) : prev(std::move(prev)), offset(offset), next(next) {}

    void prepared_remove_t::throw_exc(const std::string& msg, const c_ebmlElement_sp&) const {
        throw ebmlWriteError(msg);
    }

    template class status_t<prepared_remove_t>;
}
#endif
