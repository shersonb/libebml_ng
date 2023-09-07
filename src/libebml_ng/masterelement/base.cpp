#ifndef EBML_NG_MASTERELEMENT_CPP
#define EBML_NG_MASTERELEMENT_CPP

#include "libebml_ng/masterelement/base.h"

#include "base/initialization.cpp"
#include "base/childmgmt.cpp"
#include "base/iteration.cpp"
#include "base/size.cpp"
#include "base/encoding.cpp"
#include "base/decoding.cpp"

#include "libebml_ng/exceptions.cpp"

namespace ebml {
    const ebmlMasterElementClass* ebmlMasterElement::cls() const {
        return dynamic_cast<const ebmlMasterElementClass*>(this->_cls);
    }

    ebmlMasterElement::ebmlMasterElement(const ebmlMasterElementClass* cls) : ebmlElement(cls) {}

    void ebmlMasterElement::_clear() {
        throw ebmlNotImplementedError("_clear() not implemented.");
    }

    void ebmlMasterElement::_clonedata(const ebmlElement* elem) {
        this->_clear();

        auto melem = dynamic_cast<const ebmlMasterElement*>(elem);

        if (melem == nullptr) {
            throw std::invalid_argument("Unable to cast to const ebmlMasterElement*");
        }

        auto iter = melem->cbegin();
        auto end = melem->cend();

        while (iter != end) {
            auto childelem = *iter;
            auto copy = childelem->clone();
            this->_addChild(copy);
            ++iter;
        }
    }
}
#endif
