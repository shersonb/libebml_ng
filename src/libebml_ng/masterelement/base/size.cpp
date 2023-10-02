#ifndef EBML_NG_MASTERELEMENT_SIZETREE_CPP
#define EBML_NG_MASTERELEMENT_SIZETREE_CPP

#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/vint.h"

namespace ebml {
    sizetree_t::sizetree_t() : sizetree_t(0, 0, 0) {}

    sizetree_t::sizetree_t(sizetree_t&& other) {
        this->ebmlIDWidth = std::exchange(other.ebmlIDWidth, 0);
        this->sizeWidth = std::exchange(other.sizeWidth, 0);
        this->dataSize = std::exchange(other.dataSize, 0);
        this->children = std::move(other.children);
    }

    sizetree_t& sizetree_t::operator=(sizetree_t&& other) {
        this->ebmlIDWidth = std::exchange(other.ebmlIDWidth, 0);
        this->sizeWidth = std::exchange(other.sizeWidth, 0);
        this->dataSize = std::exchange(other.dataSize, 0);
        this->children = std::move(other.children);
        return *this;
    }

    sizetree_t::sizetree_t(unsigned char _ebmlIDWidth, unsigned char _sizeWidth, size_t _dataSize) {
        this->ebmlIDWidth = _ebmlIDWidth;
        this->sizeWidth = _sizeWidth;
        this->dataSize = _dataSize;
    }

    size_t sizetree_t::outerSize() const {
        return (this->ebmlIDWidth + this->sizeWidth + this->dataSize);
    }

    sizetree_t ebmlMasterElement::sizetree() const {
        sizetree_t result;
        // this->_build_sizetree(ret);

        auto iter = this->cbegin();
        auto end = this->cend();

        result.ebmlIDWidth = widthAsVint(this->cls()->ebmlID);
        result.dataSize = 0;

        while (iter != end) {
            auto elem = *iter;
            ++iter;

#if RAW
            auto me_elem = dynamic_cast<const ebmlMasterElement*>(elem);
#else
            auto me_elem = std::dynamic_pointer_cast<const ebmlMasterElement>(elem);
#endif

            if (me_elem != nullptr) {
                // me_elem->_build_sizetree(child_sizetree);
                sizetree_t child_sizetree = me_elem->sizetree();
                result.dataSize += (
                    child_sizetree.ebmlIDWidth
                    + child_sizetree.sizeWidth
                    + child_sizetree.dataSize);
                result.children.push_back(std::move(child_sizetree));
            } else {
                auto dataSize = elem->dataSize();
                sizetree_t child_sizetree = sizetree_t(
                    widthAsVint(elem->cls()->ebmlID),
                    widthAsVint(dataSize),
                    dataSize
                );
                result.dataSize += (
                    child_sizetree.ebmlIDWidth
                    + child_sizetree.sizeWidth
                    + child_sizetree.dataSize);
                result.children.push_back(std::move(child_sizetree));
            }
        }

        result.sizeWidth = widthAsVint(result.dataSize);
        return result;
    }

    size_t ebmlMasterElement::dataSize() const {
        // sizetree_t _sizetree;
        // this->_build_sizetree(_sizetree);
        // return _sizetree.dataSize;
        return this->sizetree().dataSize;
    }

//     void ebmlMasterElement::_build_sizetree(sizetree_t& sizetree) const {
//         auto iter = this->cbegin();
//         auto end = this->cend();
//
//         sizetree.ebmlIDWidth = widthAsVint(this->cls()->ebmlID);
//         sizetree.dataSize = 0;
//
//         while (iter != end) {
//             auto elem = *iter;
//             ++iter;
//
//             sizetree.children.emplace_back(0, 0, 0);
//             {
//                 sizetree_t& child_sizetree = sizetree.children.back();
// #if RAW
//                 auto me_elem = dynamic_cast<const ebmlMasterElement*>(elem);
// #else
//                 auto me_elem = std::dynamic_pointer_cast<const ebmlMasterElement>(elem);
// #endif
//
//                 if (me_elem != nullptr) {
//                     me_elem->_build_sizetree(child_sizetree);
//                 } else {
//                     child_sizetree.ebmlIDWidth = widthAsVint(elem->cls()->ebmlID);
//                     child_sizetree.dataSize = elem->dataSize();
//                     child_sizetree.sizeWidth = widthAsVint(child_sizetree.dataSize);
//                 }
//
//                 sizetree.dataSize += child_sizetree.ebmlIDWidth;
//                 sizetree.dataSize += child_sizetree.sizeWidth;
//                 sizetree.dataSize += child_sizetree.dataSize;
//             }
//         }
//
//         sizetree.sizeWidth = widthAsVint(sizetree.dataSize);
//     }
}
#endif
