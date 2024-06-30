#ifndef EBML_NG_MASTERELEMENT_CHILDCLASSSPEC_CPP
#define EBML_NG_MASTERELEMENT_CHILDCLASSSPEC_CPP

#include "libebml_ng/masterelement/base/childClassSpec.h"
// #include "libebml_ng/exceptions.h"
// #include "libebml_ng/elementcls.tpp"
// #include "libebml_ng/element.tpp"
// #include "libebml_ng/ptrs.h"

// #include "base/childmgmt.cpp"
// #include "base/decoding.cpp"
// #include "base/encoding.cpp"
// #include "base/initialization.cpp"
// #include "base/iteration.cpp"
// #include "base/size.cpp"

namespace ebml {
    childClassSpecArg_t::childClassSpecArg_t() {
        this->cls = nullptr;
        this->min = 0;
        this->max = -1;
    }

    childClassSpecArg_t::childClassSpecArg_t(const ebmlElementClass* cls, unsigned long min, long max) {
        this->cls = cls;
        this->min = min;
        this->max = max;
    }

    childClassSpec_t::childClassSpec_t() {}

    childClassSpec_t::childClassSpec_t(childClassSpecArg_init_l items) {
        for (const auto& item : items) {
            this->add(item);
        }
    }

    childClassSpec_t::childClassSpec_t(const childClassSpecArg_l& items) {
        for (const auto& item : items) {
            this->add(item);
        }
    }

    // childClassSpec_t::childClassSpec_t(const childClassSpec_t& other) :
    //     _cls_by_ebmlID(other._cls_by_ebmlID),
    //     _clss(other._clss),
    //     _occurspec(other._occurspec) {}

    void childClassSpec_t::add(ebml::childClassSpecArg_t const& spec) {
        if (_cls_by_ebmlID.count(spec.cls->ebmlID)) {
            throw ebmlException("Cannot specify multiple ebml::ebmlElementClass* instances managing the same ebmlID.");
        }

        _cls_by_ebmlID[spec.cls->ebmlID] = spec.cls;
        _clss.insert(spec.cls);
        _occurspec[spec.cls->ebmlID] = {spec.min, spec.max};
    }

    const occurSpec_d& childClassSpec_t::occurSpec() const {
        return this->_occurspec;
    }

    const occurSpec_t& childClassSpec_t::occurSpec(ebmlID_t ebmlID) const {
        return this->_occurspec.at(ebmlID);
    }

    const occurSpec_t& childClassSpec_t::occurSpec(const ebmlElementClass* cls) const {
        auto ebmlID = cls->ebmlID;
        return this->_occurspec.at(ebmlID);
    }

    bool childClassSpec_t::isValid(const ebmlElement_sp& elem) const {
        return this->_clss.count(&elem->cls()) != 0;
    }

    size_t childClassSpec_t::count(ebmlID_t ebmlID) const {
        return this->_cls_by_ebmlID.count(ebmlID);
    }

    size_t childClassSpec_t::size() const {
        return this->_cls_by_ebmlID.size();
    }

    const ebmlElementClass* childClassSpec_t::operator[](ebmlID_t ebmlID) const {
        if (this->_cls_by_ebmlID.count(ebmlID) == 0) {
            std::string errmsg = "No child element type matches EBML ID ";
            errmsg += std::to_string(ebmlID);
            throw ebmlNoChildMatch(errmsg, DECODE_ERR_DEFAULT, 0);
        }
        return this->_cls_by_ebmlID.at(ebmlID);
    }

    ebmlElementClass_d::const_iterator childClassSpec_t::begin() const {
        return this->_cls_by_ebmlID.cbegin();
    }

    ebmlElementClass_d::const_iterator childClassSpec_t::end() const {
        return this->_cls_by_ebmlID.cend();
    }

    childClassSpec_t& childClassSpec_t::operator=(childClassSpecArg_init_l items) {
        _clear();

        for (const auto& item : items) {
            this->add(item);
        }

        return *this;
    }

    childClassSpec_t& childClassSpec_t::operator=(const childClassSpecArg_l& items) {
        _clear();

        for (const auto& item : items) {
            this->add(item);
        }

        return *this;
    }
}
#endif
