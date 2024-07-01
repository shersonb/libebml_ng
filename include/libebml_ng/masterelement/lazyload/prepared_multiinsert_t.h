#ifndef EBML_NG_LAZYLOAD_PREPARED_MULTIINSERT_H
#define EBML_NG_LAZYLOAD_PREPARED_MULTIINSERT_H

#include <string>
#include <vector>

#include "children_m.h"
// #include "../ebmlMasterElement.h"
#include "extent_t.h"

namespace ebml {
    class prepared_multiinsert_t {
    public:
        extent_t prev;
        std::vector<extent_t> extents;
        off_t next;
        children_m::iterator hint;

        prepared_multiinsert_t();

        prepared_multiinsert_t(const extent_t&, const std::vector<extent_t>&, off_t, const children_m::iterator&);
        prepared_multiinsert_t(const extent_t&, const std::vector<extent_t>&, off_t, children_m::iterator&&);
        prepared_multiinsert_t(const extent_t&, const std::vector<extent_t>&, off_t);

        prepared_multiinsert_t(const extent_t&, std::vector<extent_t>&&, off_t, const children_m::iterator&);
        prepared_multiinsert_t(const extent_t&, std::vector<extent_t>&&, off_t, children_m::iterator&&);
        prepared_multiinsert_t(const extent_t&, std::vector<extent_t>&&, off_t);

        prepared_multiinsert_t(extent_t&&, const std::vector<extent_t>&, off_t, const children_m::iterator&);
        prepared_multiinsert_t(extent_t&&, const std::vector<extent_t>&, off_t, children_m::iterator&&);
        prepared_multiinsert_t(extent_t&&, const std::vector<extent_t>&, off_t);

        prepared_multiinsert_t(extent_t&&, std::vector<extent_t>&&, off_t, const children_m::iterator&);
        prepared_multiinsert_t(extent_t&&, std::vector<extent_t>&&, off_t, children_m::iterator&&);
        prepared_multiinsert_t(extent_t&&, std::vector<extent_t>&&, off_t);

        prepared_multiinsert_t(const prepared_multiinsert_t&) = default;
        prepared_multiinsert_t(prepared_multiinsert_t&&) = default;

        prepared_multiinsert_t& operator=(const prepared_multiinsert_t&) = default;
        prepared_multiinsert_t& operator=(prepared_multiinsert_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlInsertionError() const;
    };

    extern template class status_t<prepared_multiinsert_t>;
}

#endif
