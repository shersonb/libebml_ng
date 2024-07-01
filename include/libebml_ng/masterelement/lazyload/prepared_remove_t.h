#ifndef EBML_NG_LAZYLOAD_PREPARED_REMOVE_H
#define EBML_NG_LAZYLOAD_PREPARED_REMOVE_H

#include <string>
#include <vector>

#include "children_m.h"
// #include "../ebmlMasterElement.h"
#include "extent_t.h"
#include "status.h"

namespace ebml {
    class prepared_remove_t {
    public:
        extent_t prev;
        off_t offset;
        off_t next;
        ebmlElement_sp elem;
        children_m::iterator hint;

        prepared_remove_t();

        prepared_remove_t(const extent_t&, off_t, off_t, const ebmlElement_sp&, const children_m::iterator&);
        prepared_remove_t(const extent_t&, off_t, off_t, const ebmlElement_sp&, children_m::iterator&&);
        prepared_remove_t(const extent_t&, off_t, off_t, ebmlElement_sp&&, const children_m::iterator&);
        prepared_remove_t(const extent_t&, off_t, off_t, ebmlElement_sp&&, children_m::iterator&&);
        prepared_remove_t(const extent_t&, off_t, off_t);

        prepared_remove_t(extent_t&&, off_t, off_t, const ebmlElement_sp&, const children_m::iterator&);
        prepared_remove_t(extent_t&&, off_t, off_t, const ebmlElement_sp&, children_m::iterator&&);
        prepared_remove_t(extent_t&&, off_t, off_t, ebmlElement_sp&&, const children_m::iterator&);
        prepared_remove_t(extent_t&&, off_t, off_t, ebmlElement_sp&&, children_m::iterator&&);
        prepared_remove_t(extent_t&&, off_t, off_t);

        prepared_remove_t(const prepared_remove_t&) = default;
        prepared_remove_t(prepared_remove_t&&) = default;

        prepared_remove_t& operator=(const prepared_remove_t&) = default;
        prepared_remove_t& operator=(prepared_remove_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlMoveError() const;
    };

    extern template class status_t<prepared_remove_t>;
}

#endif
