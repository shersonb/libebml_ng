#ifndef EBML_NG_LAZYLOAD_PREPARED_RESIZE_H
#define EBML_NG_LAZYLOAD_PREPARED_RESIZE_H

#include <string>
#include "extent_t.h"
#include "status.h"
#include "children_m.h"
#include "../../seekdata.h"

namespace ebml {
    class prepared_resize_t {
    public:
        extent_t lastChild;
        off_t endOffset;
        off_t nextSibling;
        children_m::iterator hint;
        seekData_t* seekData = nullptr; // Why do we need this?

        prepared_resize_t();

        prepared_resize_t(const extent_t&, off_t, off_t);
        prepared_resize_t(const extent_t&, off_t, off_t, seekData_t*);

        prepared_resize_t(extent_t&&, off_t, off_t);
        prepared_resize_t(extent_t&&, off_t, off_t, seekData_t*);

        prepared_resize_t(const prepared_resize_t&) = default;
        prepared_resize_t(prepared_resize_t&&) = default;

        prepared_resize_t& operator=(const prepared_resize_t&) = default;
        prepared_resize_t& operator=(prepared_resize_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlResizeError() const;
    };

    extern template class status_t<prepared_resize_t>;
}

#endif
