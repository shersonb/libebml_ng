#ifndef EBML_NG_CHILDCLASSSPEC_H
#define EBML_NG_CHILDCLASSSPEC_H

// #include "../ebmlID_t.h"
// #include <cstddef>
#include "../../ebmlElementType.h"
// #include "ebmlMasterElementClass.h"
// #include "../element.h"
// #include "../exceptions.h"
//
#include <unordered_map>
#include <unordered_set>
// #include <deque>
#include <list>
// #include <vector>
// #include <optional>

namespace ebml {
    // Used in the initialization of a new ebmlMasterElementType (and its c++ subclass) instances.
    struct childTypeSpecArg_t {
        childTypeSpecArg_t();
        childTypeSpecArg_t(const ebmlElementType *, unsigned long min=0, long max=-1);
        const ebmlElementType * cls;
        unsigned long min; // Minimum number of occurrences required.
        long max; // Maximum number of occurrences permitted. Negative values interpreted as infinity.
    };

    typedef std::list<childTypeSpecArg_t> childTypeSpecArg_l;
    typedef std::initializer_list<childTypeSpecArg_t> childClassSpecArg_init_l;

    struct occurSpec_t {
        unsigned long min;
        long max;
    };

    typedef std::unordered_map<ebmlID_t, const ebmlElementType *> ebmlElementClass_d;
    typedef std::unordered_set<const ebmlElementType *> ebmlElementClass_s;
    typedef std::unordered_map<ebmlID_t, occurSpec_t> occurSpec_d;

    // Child element class specification. Used to specify accepted EBML Element types within a parent element.
    // Also used to look up EBMLElementClass* instances by ebmlID_t

    class childTypeSpec_t {
    private:
        ebmlElementClass_d _cls_by_ebmlID;
        ebmlElementClass_s _clss;
        occurSpec_d _occurspec;
        inline void _clear() {
            _cls_by_ebmlID.clear();
            _clss.clear();
            _occurspec.clear();
        }

    public:
        childTypeSpec_t();
        childTypeSpec_t(childClassSpecArg_init_l);
        childTypeSpec_t(const childTypeSpecArg_l &);
        childTypeSpec_t(const childTypeSpec_t &) = default;
        childTypeSpec_t(childTypeSpec_t &&) = default;

        childTypeSpec_t & operator=(childClassSpecArg_init_l);
        childTypeSpec_t & operator=(const childTypeSpecArg_l &);
        childTypeSpec_t & operator=(const childTypeSpec_t &) = default;
        childTypeSpec_t & operator=(childTypeSpec_t &&) = default;

        void add(const childTypeSpecArg_t &);
        void add(const ebmlElementType *, unsigned long min=0, long max=-1);
        void remove(ebmlID_t);

        const ebmlElementType * operator[](ebmlID_t) const;
        size_t count(ebmlID_t) const;
        size_t count(const ebmlElementType *) const;
        size_t size() const;

        const occurSpec_d& occurSpec() const;
        const occurSpec_t& occurSpec(ebmlID_t) const;
        const occurSpec_t& occurSpec(const ebmlElementType *) const;

        bool isValid(const ebmlElement_sp&) const;

        ebmlElementClass_d::const_iterator begin() const;
        ebmlElementClass_d::const_iterator end() const;
    };
}
#endif
