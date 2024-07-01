#ifndef EBML_NG_LAZYLOAD_EXTENTS_H
#define EBML_NG_LAZYLOAD_EXTENTS_H

#include <sys/types.h>
#include "../base/sizetree_t.h"
#include "../../ebmlID_t.h"

namespace ebml {
    struct extent_t {
        off_t offset;
        sizetree_t sizetree;
        off_t endOffset;

        extent_t();
        extent_t(off_t);
        extent_t(off_t, vintWidth_t, vintWidth_t, size_t);
        extent_t(off_t, const sizetree_t&);
        extent_t(off_t, sizetree_t&&);

        extent_t(const extent_t&) = default;
        extent_t(extent_t&&) = default;

        extent_t& operator=(const extent_t&) = default;
        extent_t& operator=(extent_t&&) = default;
    };
}
#endif
