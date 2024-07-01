#ifndef EBML_NG_LAZYLOAD_EXTENT_CPP
#define EBML_NG_LAZYLOAD_EXTENT_CPP

#include "libebml_ng/masterelement/lazyload/extent_t.h"
#include "libebml_ng/vint.h"
#include <cstddef>

namespace ebml {
    extent_t::extent_t() : offset(0), sizetree(0, 0, 0), endOffset(0) {}

    extent_t::extent_t(off_t offset) : offset(offset), sizetree(0, 0, 0), endOffset(offset) {}

    extent_t::extent_t(off_t offset, const ebml::sizetree_t& sizetree) : offset(offset), sizetree(sizetree), endOffset((sizetree.dataSize != UNKNOWN) ? offset + sizetree.outerSize() : UNKNOWN) {}

    extent_t::extent_t(off_t offset, ebml::sizetree_t&& sizetree) : offset(offset), sizetree(std::move(sizetree)), endOffset((this->sizetree.dataSize != UNKNOWN) ? offset + this->sizetree.outerSize() : UNKNOWN) {}

}
#endif
