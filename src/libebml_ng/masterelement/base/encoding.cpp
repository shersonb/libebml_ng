#ifndef EBML_NG_MASTERELEMENT_ENCODE_CPP
#define EBML_NG_MASTERELEMENT_ENCODE_CPP

#include <string>
#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/exceptions.h"

namespace ebml {
    std::string ebmlMasterElement::encode() const {
        std::string result;
        auto _sizetree = this->sizetree();
        result.resize(_sizetree.outerSize());
        this->encode(&result[0], _sizetree);
        return result;
    }

    size_t ebmlMasterElement::encode(char* dest) const {
        auto _sizetree = this->sizetree();
        return this->encode(dest, _sizetree);
    }

    size_t ebmlMasterElement::encode(char* dest, const sizetree_t& _sizetree) const {
        size_t headWidth = this->_encode_head(dest, _sizetree.dataSize);

        if (this->_encode(dest + headWidth, _sizetree) != _sizetree.dataSize) {
            throw ebmlEncodeError("Encoded size does not match expected size.", this->shared_from_this());
        };

        return headWidth + _sizetree.dataSize;
    }


    size_t ebmlMasterElement::_encode(char* dest) const {
        auto _sizetree = this->sizetree();
        return this->_encode(dest, _sizetree);
    }

    size_t ebmlMasterElement::_encode(char* dest, const sizetree_t& _sizetree) const {
        size_t offset = 0;

        auto elem_iter = this->cbegin();
        auto elem_end = this->cend();

        auto tree_iter = _sizetree.children.cbegin();
        auto tree_end = _sizetree.children.cend();

        while (elem_iter != elem_end) {
            if (tree_iter == tree_end) {
                throw std::runtime_error("Unexpected end of branch.");
            }
            auto elem = *elem_iter;
#if RAW
            auto me_elem = dynamic_cast<const ebmlMasterElement*>(elem);
#else
            auto me_elem = std::dynamic_pointer_cast<const ebmlMasterElement>(elem);
#endif

            if (me_elem != nullptr) {
                offset += me_elem->encode(dest + offset, *tree_iter);
            } else {
                offset += elem->encode(dest + offset, (*tree_iter).dataSize);
            }

            ++elem_iter;
            ++tree_iter;
        }

        if (tree_iter != tree_end) {
            throw std::runtime_error("Branch continues past expected end.");
        }

        return offset;
    }
}
#endif
