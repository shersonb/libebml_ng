#ifndef EBML_NG_LAZYLOAD_PREPARED_FALLOCATE_H
#define EBML_NG_LAZYLOAD_PREPARED_FALLOCATE_H

#include "extent_t.h"
#include "children_m.h"

#ifdef __linux__
    #include <fcntl.h>

    #ifndef HAS_FALLOCATE
        #ifdef _GNU_SOURCE
            #define HAS_FALLOCATE 1
        #else
            #define HAS_FALLOCATE 0
        #endif
    #endif

#else
    #define HAS_FALLOCATE 0
#endif

namespace ebml {
#if HAS_FALLOCATE
    class prepared_fallocate_t {
    public:
        extent_t prev;
        off_t nextOld;
        off_t nextNew;
        children_m::iterator hint;

        prepared_fallocate_t();

        prepared_fallocate_t(const extent_t&, off_t, off_t);
        prepared_fallocate_t(extent_t&&, off_t, off_t);
        prepared_fallocate_t(const extent_t&, off_t, off_t, const children_m::iterator&);
        prepared_fallocate_t(extent_t&&, off_t, off_t, const children_m::iterator&);
        prepared_fallocate_t(const extent_t&, off_t, off_t, children_m::iterator&&);
        prepared_fallocate_t(extent_t&&, off_t, off_t, children_m::iterator&&);
        prepared_fallocate_t(const prepared_fallocate_t&) = default;
        prepared_fallocate_t(prepared_fallocate_t&&) = default;

        prepared_fallocate_t& operator=(const prepared_fallocate_t&) = default;
        prepared_fallocate_t& operator=(prepared_fallocate_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlInsertionError() const;
    };

    extern template class status_t<prepared_fallocate_t>;
#endif
}

#endif
