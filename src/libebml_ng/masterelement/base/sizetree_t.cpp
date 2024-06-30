#ifndef EBML_NG_MASTERELEMENT_SIZETREE_CPP
#define EBML_NG_MASTERELEMENT_SIZETREE_CPP

#include "libebml_ng/masterelement/base/sizetree_t.h"
#include <utility>

namespace ebml {
    sizetree_t::sizetree_t() : sizetree_t(0, 0, 0) {}

    sizetree_t::sizetree_t(const sizetree_t& other) : ebmlIDWidth(other.ebmlIDWidth), sizeWidth(other.sizeWidth), dataSize(other.dataSize), children(other.children) {}

    sizetree_t::sizetree_t(sizetree_t&& other) : ebmlIDWidth(std::exchange(other.ebmlIDWidth, 0)), sizeWidth(std::exchange(other.sizeWidth, 0)), dataSize(std::exchange(other.dataSize, 0)), children(std::move(other.children)) {}

    sizetree_t::sizetree_t(unsigned char _ebmlIDWidth, unsigned char _sizeWidth, size_t _dataSize)
    : ebmlIDWidth(_ebmlIDWidth), sizeWidth(_sizeWidth), dataSize(_dataSize) {}

    sizetree_t& sizetree_t::operator=(const sizetree_t& other) {
        this->ebmlIDWidth = other.ebmlIDWidth;
        this->sizeWidth = other.sizeWidth;
        this->dataSize = other.dataSize;
        this->children = other.children;
        return *this;
    }

    sizetree_t& sizetree_t::operator=(sizetree_t&& other) {
        this->ebmlIDWidth = std::exchange(other.ebmlIDWidth, 0);
        this->sizeWidth = std::exchange(other.sizeWidth, 0);
        this->dataSize = std::exchange(other.dataSize, 0);
        this->children = std::move(other.children);
        return *this;
    }

    void sizetree_t::append(const sizetree_t& other) {
        dataSize += (
            other.ebmlIDWidth
            + other.sizeWidth
            + other.dataSize);
        if (children.has_value()) {
            children->push_back(other);
        } else {
            children = {other};
        }
    }

    void sizetree_t::append(sizetree_t&& other) {
        dataSize += (
            other.ebmlIDWidth
            + other.sizeWidth
            + other.dataSize);
        // children.push_back(std::move(other));
        if (children.has_value()) {
            children->push_back(std::move(other));
        } else {
            children = {std::move(other)};
        }
    }

    size_t sizetree_t::outerSize() const {
        return (ebmlIDWidth + sizeWidth + dataSize);
    }
}
#endif
