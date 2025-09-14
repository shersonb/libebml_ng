#ifndef EBML_NG_SIZETREE_H
#define EBML_NG_SIZETREE_H

#include "../../ebmlID_t.h"
#include <deque>
#include <optional>

namespace ebml {
    /**
     * @brief Structure representing the size tree for an EBML master element.
     *
     * The size tree captures the widths for the EBML ID and size fields as well as
     * the overall data size. It may also include a list of child size trees.
     */
    struct sizetree_t {
        vintWidth_t ebmlIDWidth;
        vintWidth_t sizeWidth;
        size_t dataSize;
        // std::deque<sizetree_t> children;
        std::optional<std::deque<sizetree_t>> children;

        sizetree_t();
        sizetree_t(const sizetree_t&);
        sizetree_t(sizetree_t&&);
        sizetree_t(vintWidth_t _ebmlIDWidth, vintWidth_t _sizeWidth, size_t _dataSize);
        sizetree_t& operator=(const sizetree_t&);
        sizetree_t& operator=(sizetree_t&&);
        size_t outerSize() const;
        void append(const sizetree_t&);
        void append(sizetree_t&&);
    };
}
#endif
