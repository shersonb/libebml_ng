#ifndef EBML_NG_MASTERELEMENT_C_EBMLELEMENT_L_H
#define EBML_NG_MASTERELEMENT_C_EBMLELEMENT_L_H

#include "../../ebmlElement.h"

#include <vector>

namespace ebml {
    typedef std::vector<ebmlElement_sp> ebmlElement_l;

    class c_ebmlElement_l {
    private:
        const ebmlElement_l* _items;

    public:
        c_ebmlElement_l(const ebmlElement_l&);
        c_ebmlElement_l(const c_ebmlElement_l&) = default;

        const ebmlElement& front() const;
        const ebmlElement& back() const;
        const ebmlElement& at(size_t) const;
        const ebmlElement& operator[](size_t) const;

        size_t size() const;
        bool empty() const;

        class const_iterator {
        private:
            ebmlElement_l::const_iterator _iter;

        protected:
            const_iterator(const ebmlElement_l::const_iterator&);
            const_iterator(ebmlElement_l::const_iterator&&);

        public:
            const_iterator(const c_ebmlElement_l&);
            const_iterator(const const_iterator&);
            const_iterator(const_iterator&&);

            bool operator=(const const_iterator&);
            bool operator=(const_iterator&&);

            const ebmlElement& operator*() const;

            const ebmlElement* operator->() const;
            const_iterator& operator++();
            const_iterator operator++(int);
            bool operator==(const const_iterator&) const;
            bool operator!=(const const_iterator&) const;
        };

        const_iterator cbegin() const;
        const_iterator cend() const;
    };
}
#endif
