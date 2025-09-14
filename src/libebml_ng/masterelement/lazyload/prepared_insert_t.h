#ifndef EBML_NG_LAZYLOAD_PREPARED_INSERT_H
#define EBML_NG_LAZYLOAD_PREPARED_INSERT_H

#include <string>
#include <vector>

#include "children_m.h"
// #include "../ebmlMasterElement.h"
#include "extent_t.h"

namespace ebml {
    class prepared_insert_t {
    public:
        extent_t prev;
        extent_t extent;
        off_t next;
        children_m::iterator hint;

        prepared_insert_t();

        prepared_insert_t(const extent_t&, const extent_t&, off_t);
        prepared_insert_t(const extent_t&, extent_t&&, off_t);
        prepared_insert_t(extent_t&&, const extent_t&, off_t);
        prepared_insert_t(extent_t&&, extent_t&&, off_t);

        prepared_insert_t(const extent_t&, const extent_t&, off_t, const children_m::iterator&);
        prepared_insert_t(const extent_t&, const extent_t&, off_t, children_m::iterator&&);
        prepared_insert_t(const extent_t&, extent_t&&, off_t, const children_m::iterator&);
        prepared_insert_t(const extent_t&, extent_t&&, off_t, children_m::iterator&&);
        prepared_insert_t(extent_t&&, const extent_t&, off_t, const children_m::iterator&);
        prepared_insert_t(extent_t&&, const extent_t&, off_t, children_m::iterator&&);
        prepared_insert_t(extent_t&&, extent_t&&, off_t, const children_m::iterator&);
        prepared_insert_t(extent_t&&, extent_t&&, off_t, children_m::iterator&&);

        // prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t, const children_m::iterator&);
        // prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t, children_m::iterator&&);
        // prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t, const children_m::iterator&);
        // prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t, children_m::iterator&&);
        // prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t);
        // prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t);
        // ~prepared_insert_t();

        prepared_insert_t(const prepared_insert_t&) = default;
        prepared_insert_t(prepared_insert_t&&) = default;

        prepared_insert_t& operator=(const prepared_insert_t&) = default;
        prepared_insert_t& operator=(prepared_insert_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlInsertionError() const;
    };

    extern template class status_t<prepared_insert_t>;
}

#endif
