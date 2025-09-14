#ifndef EBML_NG_ELEMENT_TPP
#define EBML_NG_ELEMENT_TPP

#include "ebmlElement.h"
#include "masterelement/base/ebmlMasterElement.h"

namespace ebml {
    template<typename T>
    inline ebml::ptr<T> ebmlElement::sp() {
        return _self.lock<T>();
    }

    template<typename T>
    inline ebml::ptr<T> ebmlElement::sp() const {
        return _self.c_lock<T>();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    const ebmltype_t& ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::cls() const {
        return *static_cast<const ebmltype_t*>(this->_cls);
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebmlElement* ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::_clone() const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            auto recast = static_cast<const ebmlinst_t*>(this);

            if constexpr (has_constructor<ebmlinst_t, const ebmlinst_t&>) {
                auto clone = new ebmlinst_t(*recast);
                clone->_validate();
                return clone;
            } else if constexpr (has_clone_method<ebmlinst_t>) {
                auto clone = new ebmlinst_t(&recast->cls());
                clone->_validate();
                clone->_clone(*recast);
                return clone;
            } else {
                throw std::runtime_error("No cloning logic defined.");
            }
        } else {
            return nullptr;
        }
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebml::ptr<ebmlinst_t> ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::sp() {
        return this->_self.template lock<ebmlinst_t>();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebml::ptr<const ebmlinst_t> ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::sp() const {
        return this->_self.template lock<const ebmlinst_t>();
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebml::wptr<ebmlinst_t> ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::wp() {
        return ebml_static_pointer_cast<ebmlinst_t>(this->_self);
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebml::wptr<const ebmlinst_t> ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::wp() const {
        return ebml_static_pointer_cast<const ebmlinst_t>(this->_self);
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t>
    ebml::ptr<ebmlinst_t> ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>::clone() const {
        auto clone = _clone();
        return static_cast<ebmlinst_t*>(clone)->sp();
    }
}
#endif
